#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"

#include "engine.hpp"
#include "messages.hpp"

namespace VPanic {
	
	Engine::Engine() {}
	Engine::~Engine() { quit(); }


	void Engine::set_update_callback(void(*t_callback)()) {
		m_update_callback = t_callback;
	}
	
	void Engine::set_mouse_wheel_callback(void(*t_callback)(uint8_t)) {
		m_mouse_wheel_callback = t_callback;
	}
	
	void Engine::set_keydown_callback(void(*t_callback)(uint8_t)) {
		m_keydown_callback = t_callback;
	}
	
	void Engine::set_camera(Camera* t_cam) {
		m_cam = t_cam;
	}
	
	void Engine::init(const char* t_title, const glm::vec2& t_size, const int t_settings) {
		if(m_init_ok) { 
			message(MType::WARNING, "Already initialized!");
			return;
	   	}

		message(MType::INFO, "Initializing VirtualPanic ...");

		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			message(MType::BAD, "Failed to initialize SDL! (%s)", SDL_GetError());
			return;
		}
		
		message(MType::OK, "Initialized SDL2");

		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);	
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);	
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);


		m_window = SDL_CreateWindow(t_title, 0, 0, t_size.x, t_size.y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);	
		
		glEnable(GL_MULTISAMPLE);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // TODO: option for this!
		
		if(m_window == nullptr) {
			message(MType::BAD, "Failed to create window! (%s)", SDL_GetError());
			quit();
			return;	
		}

		m_context = SDL_GL_CreateContext(m_window);
		SDL_GetWindowSize(m_window, &m_width, &m_height);
		
		
		message(MType::OK, "Created new window (%4%ix%i%0)", m_width, m_height);

		int glew_err = glewInit();
		if(glew_err != GLEW_OK) {
			message(MType::BAD, "Failed to initialize glew! (%s)", SDL_GetError());
			quit();
			return;
		}
		
		message(MType::OK, "Initialized glew");
		
		glViewport(0, 0, m_width, m_height);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		
		glEnable(GL_DEPTH_TEST);
	
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		message(MType::DEBUG, "%s", glGetString(GL_VERSION));
		message(MType::DEBUG, "GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

		glClearColor(background_color.r / 255.0f, background_color.g / 255.0f, background_color.b / 255.0f, 1.0f);
		SDL_GL_SwapWindow(m_window);

		camera_active(true);
	

		// check flags
		
		if(t_settings & INIT_IMGUI) {
			m_using_imgui = true;
			ImGui::CreateContext();
			ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
			ImGui_ImplOpenGL3_Init("#version 130"); // TODO: add options for this

			message(MType::OK, "Initialized ImGui");
		}

		if(t_settings & FULLSCREEN) {
			message(MType::INFO, "Using Fullscreen");
			fullscreen(true);
		}

		if(t_settings & VSYNC) {
			message(MType::INFO, "Using VSync");
			vsync(true);
		}

		message(MType::OK, "Done!");
		m_init_ok = true;
	
	}

	void Engine::request_shutdown() {
		m_quit = true;
	}

	void Engine::quit() {
		if(m_quit && !m_init_ok) { return; }

		// cleanup some memory,  TODO: check for memory leaks

		message(MType::INFO, "Quitting...");

		camera_active(false);

		if(m_using_imgui) {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
			message(MType::OK, "Destroyed ImGui");
		}

		if(m_context != NULL) {
			SDL_GL_DeleteContext(m_context);
			message(MType::OK, "Destroyed context");
		}
		
		if(m_window != nullptr) {
			SDL_DestroyWindow(m_window);
			message(MType::OK, "Destroyed window");
		}

		SDL_Quit();
		
		m_update_callback = nullptr;
		m_keydown_callback = nullptr;
		m_mouse_wheel_callback = nullptr;
	   	m_window = nullptr;
		m_context = NULL;
		m_cam = nullptr;

		m_using_imgui = false;
		m_using_camera = false;
		m_init_ok = false;
		m_quit = true;

		message(MType::INFO, "Engine cleanup done!");
		// NOTE: some stuff may have calls in their destructor when exiting!
	}

	void Engine::start() {
		if(!ok()) { return; }
		if(m_loop) { return; }
		m_loop = true;
		
		SDL_WarpMouseInWindow(m_window, m_width/2, m_height/2);
		glm::vec2 p_mpos(m_width/2, m_height/2);
		if(m_cam != nullptr) {
			m_cam->yaw = -90.f;
		}

		Timer timer;
		SDL_Event event;
		while(!m_quit) {
			if(!ok()) { break; }

			glClearColor(
					background_color.r / 255.0f,
				   	background_color.g / 255.0f,
				   	background_color.b / 255.0f,
				   	1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if(m_using_imgui) {
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplSDL2_NewFrame(m_window);
				ImGui::NewFrame();
			}

			if(m_cam != nullptr && m_using_camera) {
				
				if(Keyboard::keydown('w')) {
					m_cam->pos += m_cam->front * m_cam->move_speed;
				}
				if(Keyboard::keydown('a')) {
					m_cam->pos -= glm::normalize(glm::cross(m_cam->front, glm::vec3(0.0f, 1.0f, 0.0f))) * m_cam->move_speed;
				}
				if(Keyboard::keydown('s')) {
					m_cam->pos -= m_cam->front * m_cam->move_speed;
				}
				if(Keyboard::keydown('d')) {
					m_cam->pos += glm::normalize(glm::cross(m_cam->front, glm::vec3(0.0f, 1.0f, 0.0f))) * m_cam->move_speed;
				}
				if(Keyboard::keydown(Keyboard::Mod::SPACE)) {
					m_cam->pos.y += m_cam->move_speed;
				}
				else if(Keyboard::keydown(Keyboard::Mod::SHIFT)) {
					m_cam->pos.y -= m_cam->move_speed;
				}
			}
			
			if(m_update_callback != nullptr) {
				m_update_callback();
			}

			if(m_using_imgui) {
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}

			// handle events
			while(SDL_PollEvent(&event)) {
				switch(event.type) {
					case SDL_QUIT:
						quit();
						break;

					case SDL_WINDOWEVENT:
						switch(event.window.event) {
							case SDL_WINDOWEVENT_SIZE_CHANGED:
								message(MType::DEBUG, "New window size: %ix%i", event.window.data1, event.window.data2);
								m_width = event.window.data1;
								m_height = event.window.data2;
								glViewport(0, 0, m_width, m_height);
								break;
						}
						break;

					case SDL_MOUSEMOTION:
						if(m_cam != nullptr && m_using_camera) {
							m_cam->yaw += static_cast<float>(event.motion.xrel) * m_cam->sensetivity;
							m_cam->pitch += static_cast<float>(-event.motion.yrel) * m_cam->sensetivity;
							clamp<float>(m_cam->pitch, -89.0, 89.0);
							SDL_WarpMouseInWindow(m_window, m_width/2, m_height/2);
						}
						break;

					case SDL_MOUSEWHEEL:
						if(m_mouse_wheel_callback != nullptr) {
							m_mouse_wheel_callback(event.wheel.y);
						}
						break;
					
					case SDL_KEYDOWN:
						if(m_keydown_callback != nullptr) {
							m_keydown_callback(event.key.keysym.sym);
						}
						break;


					default: break;
				}
			}

			SDL_GL_SwapWindow(m_window);
		}
		m_loop = false;
		quit();
	}

	bool Engine::ok() const {
		return (m_init_ok && m_window != nullptr && m_context != NULL && !m_quit);
	}
	
	float Engine::get_aratio() const {
		return static_cast<float>(m_width) / static_cast<float>(m_height);
	}
		
	glm::vec2 Engine::get_window_size() const {
		return glm::vec2(m_width, m_height);
	}

	void Engine::camera_active(const bool b) {
		SDL_SetWindowGrab(m_window, b ? SDL_TRUE : SDL_FALSE);
		SDL_SetRelativeMouseMode(b ? SDL_TRUE : SDL_FALSE);
		SDL_ShowCursor(b);
		SDL_WarpMouseInWindow(m_window, m_width/2, m_height/2);
		m_using_camera = b;
	}

	void Engine::vsync(const bool b) {
		if(SDL_GL_SetSwapInterval(b)) {
			message(MType::BAD, "Error with VSync! (%s)", SDL_GetError());
		}
	}

	void Engine::fullscreen(const bool b) {
		SDL_SetWindowFullscreen(m_window, b ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	}

}







