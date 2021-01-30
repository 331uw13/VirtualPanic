#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include "engine.hpp"
#include "messages.hpp"

namespace VPanic {
	
	Engine::Engine() {}
	Engine::~Engine() { quit(); }


	void Engine::set_update_callback(void(*t_callback)()) {
		m_update_callback = t_callback;
	}
	
	void Engine::set_mousewheel_callback(void(*t_callback)(uint8_t)) {
		m_mousewheel_callback = t_callback;
	}
	
	void Engine::set_keydown_callback(void(*t_callback)(uint8_t)) {
		m_keydown_callback = t_callback;
	}
	
	void Engine::set_camera(Camera* t_cam) {
		m_cam = t_cam;
	}
	
	void Engine::init(const char* t_title, const vec2& t_size) {
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
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		
		if(m_window == nullptr) {
			message(MType::BAD, "Failed to create window! (%s)", SDL_GetError());
			quit();
			return;	
		}

		m_context = SDL_GL_CreateContext(m_window);
		SDL_GetWindowSize(m_window, &m_width, &m_height);
		
		
		message(MType::OK, "Created new window (%4%ix%i%0)", m_width, m_height);

		if(SDL_GL_SetSwapInterval(1)) {
			message(MType::BAD, "Failed to enable VSync! (%s)", SDL_GetError());
		}

		int glew_err = glewInit();
		if(glew_err != GLEW_OK) {
			message(MType::BAD, "Failed to initialize glew! (%s)", SDL_GetError());
			quit();
			return;
		}
		
		message(MType::OK, "Initialized glew");
		
		glViewport(0, 0, m_width, m_height);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		message(MType::DEBUG, "%s", glGetString(GL_VERSION));
		message(MType::DEBUG, "GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_ShowCursor(false);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		message(MType::OK, "Done!");
		m_init_ok = true;
	}

	void Engine::quit() {
		if(m_quit && !m_init_ok) { return; }

		// cleanup memory, TODO: check for memory leaks
		message(MType::INFO, "Quitting...");

		if(m_window != nullptr) {
			SDL_DestroyWindow(m_window);
	   		m_window = nullptr;
			message(MType::OK, "Deleted window");
		}

		if(m_context != NULL) {
			SDL_GL_DeleteContext(m_context);
			m_context = NULL;
			message(MType::OK, "Deleted context");
		}
		SDL_Quit();
		
		m_update_callback = nullptr;
		m_quit = true;
		m_init_ok = false;

		message(MType::INFO, "Engine cleanup done!");
		// NOTE: some stuff may have calls in their destructor when exiting!
	}	

	void Engine::execute() {
		if(!ok()) { return; }
		if(m_loop) { return; }
		m_loop = true;
		
		SDL_WarpMouseInWindow(m_window, m_width/2, m_height/2);
		vec2 p_mpos(m_width/2, m_height/2);
		if(m_cam != nullptr) {
			m_cam->yaw = -90.f;
		}
	
		//float mx = -90.f;
		//float my = 0;


		Timer timer;
		SDL_Event event;
		while(!m_quit) {
			glClearColor(
					background_color.r / 255.0f,
				   	background_color.g / 255.0f,
				   	background_color.b / 255.0f,
				   	1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			if(m_cam != nullptr) {
				
				if(Keyboard::keydown('w')) {
					m_cam->pos += m_cam->move_speed * m_cam->front;
				}
				if(Keyboard::keydown('a')) {
					m_cam->pos -= glm::normalize(glm::cross(m_cam->front, glm::vec3(0.0f, 1.0f, 0.0f))) * m_cam->move_speed;
				}
				if(Keyboard::keydown('s')) {
					m_cam->pos -= m_cam->move_speed * m_cam->front;
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
						if(m_cam != nullptr) {
							m_cam->yaw += static_cast<float>(event.motion.xrel) * m_cam->sensetivity;
							m_cam->pitch += static_cast<float>(-event.motion.yrel) * m_cam->sensetivity;
							clamp<float>(m_cam->pitch, -89.0, 89.0);
						}
						break;

					case SDL_MOUSEWHEEL:
						if(m_mousewheel_callback != nullptr) {
							m_mousewheel_callback(event.wheel.y);
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
			
			if(m_update_callback != nullptr) {
				m_update_callback();
			}

			SDL_GL_SwapWindow(m_window);
		}
		m_loop = false;
	}

	bool Engine::ok() {
		return (m_init_ok && m_window != nullptr);
	}
	
	float Engine::get_aratio() const {
		return static_cast<float>(m_width) / static_cast<float>(m_height);
	}
		
	vec2 Engine::get_window_size() const {
		return vec2(m_width, m_height);
	}

	vec2 Engine::get_window_center() const {
		return get_window_size() / 2;
	}

	void Engine::fullscreen(const bool b) {
		SDL_SetWindowFullscreen(m_window, b ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	}

}







