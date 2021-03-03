#include <GL/gl3w.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include "libs/imgui/imgui_impl_opengl3.h"
#include "libs/imgui/imgui_impl_sdl.h"

#include "engine.hpp"
#include "messages.hpp"
#include "timer.hpp"
#include "console.hpp"


namespace vpanic {
	
	Engine::Engine() {}
	Engine::~Engine() { quit(); }

	void Engine::mouse_move_callback(void(*t_callback)(const MouseData&)) {
		m_mouse_move_callback = t_callback;
	}

	void Engine::mouse_wheel_callback(void(*t_callback)(int8_t)) {
		m_mouse_wheel_callback = t_callback;
	}

	void Engine::keydown_callback(void(*t_callback)(uint8_t)) {
		m_keydown_callback = t_callback;
	}

	void Engine::update_callback(void(*t_callback)()) {
		m_update_callback = t_callback;
	}
	
	void Engine::use_camera(Camera* t_cam_ptr) {
		m_cam = t_cam_ptr;
	}

	EngineState Engine::copy_state() const {
		return m_state;
	}

	EngineState& Engine::get_state_ref() {
		return m_state;
	}

	void Engine::_update_engine_ok_state() {
		if(m_window == nullptr || m_context == NULL || !m_state[EngineState::INIT_OK]) {
			m_state.unset(EngineState::OK);
		}
		else {
			m_state.set(EngineState::OK);
		}
	}

	void Engine::init(const char* t_title, const glm::vec2& t_size, const int t_settings) {
		if(m_state[EngineState::INIT_OK]) { 
			message(MType::WARNING, "Already initialized?!");
			return;
	   	}

		m_state.clear();
		Timer timer;
		message(MType::INFO, "Hello!");

		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			message(MType::ERROR, "Failed to initialize SDL (%s)", SDL_GetError());
			return;
		}	
		message(MType::OK, "Initialized SDL");

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);	
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		m_window = SDL_CreateWindow(t_title, 0, 0, t_size.x, t_size.y,
			   	SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);	
		
		if(m_window == nullptr) {
			message(MType::ERROR, "Failed to create window! (%s)", SDL_GetError());
			quit();
			return;	
		}
		
		message(MType::OK, "Created new window");
		
		//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		m_context = SDL_GL_CreateContext(m_window);
		SDL_GL_MakeCurrent(m_window, m_context);
	
		if(m_context != NULL) {
			message(MType::OK, "Created context");
		}

		if(gl3wInit()) {
			message(MType::ERROR, "Failed to initialize gl3w!");
			quit();
			return;		
		}

		message(MType::OK, "Initialized gl3w");	
		/*if(!gl3wIsSupported(3, 3)) {
			message(MType::BAD, "Current OpenGL version is not supported!!!");
			quit();
			return;
		}*/

	
		//glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		winding_order(CLOCKWISE);

		glStencilFunc(GL_ALWAYS, 1, 255);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0);

		message(MType::INFO, "%s", glGetString(GL_VERSION));
		message(MType::INFO, "GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

		glClearColor(background_color.r/255.0f, background_color.g/255.0f, background_color.b/255.0f, 1.0f);
		SDL_GL_SwapWindow(m_window);

		ImGui::CreateContext();
		ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
		ImGui_ImplOpenGL3_Init("#version 330");
		
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		message(MType::OK, "Initialized ImGui");


		if(t_settings & FULLSCREEN) {
			message(MType::INFO, "Using Fullscreen");
			fullscreen(true);
		}

		if(t_settings & VSYNC) {
			message(MType::INFO, "Using VSync");
			vsync(true);
		}
		
		SDL_GetWindowSize(m_window, &m_width, &m_height);
		glViewport(0, 0, m_width, m_height);

		message(MType::OK, "Engine is ready! [%ims]", timer.elapsed_ms());
		m_state.set(EngineState::INIT_OK);
		_update_engine_ok_state();
	}

	void Engine::request_shutdown() {
		m_state.set(EngineState::QUIT);
	}

	void Engine::quit() {
		if(!m_state[EngineState::QUIT] && !m_state[EngineState::KEEP_LOOP]) {
			return;
	   	}

		// cleanup some memory,  TODO: check for memory leaks

		message(MType::INFO, "Quitting...");

		unload_skybox();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		message(MType::OK, "Unloaded ImGui");

		if(m_context != NULL) {
			SDL_GL_DeleteContext(m_context);
			message(MType::OK, "Destroyed context");
		}
		
		if(m_window != nullptr) {
			SDL_DestroyWindow(m_window);
			message(MType::OK, "Destroyed window");
		}

		SDL_Quit();	
		message(MType::OK, "Destroyed SDL");
		message(MType::INFO, "Reset settings and state");	
		
		m_update_callback        = nullptr;
		m_keydown_callback       = nullptr;
		m_mouse_wheel_callback   = nullptr;
		m_mouse_move_callback    = nullptr;
	   	m_window   = nullptr;
		m_context  = NULL;

		m_state.clear();
		message(MType::INFO, "Engine cleanup done!");
		// NOTE: some stuff may have calls in their destructor when exiting!
	}

	void Engine::start() {
		if(!m_state[EngineState::OK]) { return; }
		if(m_state[EngineState::KEEP_LOOP]) { return; }
		m_state.set(EngineState::KEEP_LOOP);
		
		SDL_WarpMouseInWindow(m_window, m_width/2, m_height/2);

		Timer timer;
		SDL_Event event;

		const uint32_t res_x = 230*2;
		const uint32_t res_y = 170*2;


		while(!m_state[EngineState::QUIT]) {
			_update_engine_ok_state();
			if(!m_state[EngineState::OK]) { break; }

			glClearColor(
					background_color.r / 255.0f,
				   	background_color.g / 255.0f,
				   	background_color.b / 255.0f,
				   	1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			if(m_cam != nullptr) {
				// update camera and set its view, projection and position for everyone to use
				m_cam->update();
				glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
				// NOTE: projection probably doesnt change that much, create event for it?
				// NOTE: i can have array of stuff what should be updated here
				glBufferSubData(GL_UNIFORM_BUFFER, 0,                     sizeof(glm::mat4),   &(m_cam->projection*m_cam->view)[0][0]);
				glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),     sizeof(glm::vec3),   &m_cam->pos);
			}
		
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(m_window);
			ImGui::NewFrame();
		
			glStencilMask(0);

			if(m_skybox.texture.is_loaded() && m_cam != nullptr) { 
				
				glDepthFunc(GL_LEQUAL);
				_needs_render_back(true);

				m_skybox.shader.use();
				m_skybox.shader.set_mat4("matrix", m_cam->projection*glm::mat4(glm::mat3(m_cam->view)));
				m_skybox.texture.enable();
				m_skybox.shape.draw(m_skybox.shader);
				glUseProgram(0);
				m_skybox.texture.disable();
			
				_needs_render_back(false);	
				glDepthFunc(GL_LESS);
			}

			
			if(m_update_callback != nullptr) {
				m_update_callback();
			}

			/*
			glViewport(0, 0, res_x, res_y);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, res_x, res_y, 0, 0, m_width, m_height, 
					GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
			*/

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// handle events
			while(SDL_PollEvent(&event)) {
				ImGui_ImplSDL2_ProcessEvent(&event);
				switch(event.type) {
					case SDL_QUIT:
						quit();
						break;

					case SDL_WINDOWEVENT:
						if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
							message(MType::DEBUG, "New window size: %ix%i", event.window.data1, event.window.data2);
							m_width = event.window.data1;
							m_height = event.window.data2;
							glViewport(0, 0, m_width, m_height);
						}
						break;

					case SDL_MOUSEMOTION:
						if(m_mouse_move_callback != nullptr) {
							m_mouse_move_callback(MouseData{
										static_cast<float>(event.motion.x), static_cast<float>(event.motion.y),
										static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel)});
						}
						if(m_state[EngineState::LOCK_MOUSE]) {
							SDL_WarpMouseInWindow(m_window, m_width/2, m_height/2);
						}
						break;

					case SDL_MOUSEWHEEL:
						if(m_mouse_wheel_callback != nullptr) {
							m_mouse_wheel_callback(event.wheel.y);
						}
						break;
					
					case SDL_KEYDOWN:
						//message(MType::DEBUG, "KEY: %s", SDL_GetKeyName(event.key.keysym.sym));	
						if(m_keydown_callback != nullptr) {
							m_keydown_callback(event.key.keysym.sym);
						}
						break;


					default: break;
				}
			}
			SDL_GL_SwapWindow(m_window);
		}

		m_state.unset(EngineState::KEEP_LOOP);
		quit();
	}

	float Engine::get_aratio() const {
		return static_cast<float>(m_width) / static_cast<float>(m_height);
	}
		
	glm::vec2 Engine::get_window_size() const {
		return glm::vec2(m_width, m_height);
	}

	bool Engine::load_skybox(const std::vector<const char*> t_files) {
		if(t_files.empty()) { return false; }
		if(!m_skybox.texture.load_cube(t_files)) {
			return false;
		}

		std::vector<Vertex> data;
		add_box_data(data);
		m_skybox.shape.load(data);

		const char* vertex_src = 
			"#version 330 core\n"
			"layout (location = 0) in vec3 pos;"
			"uniform mat4 model;"
			"uniform mat4 matrix;"
			"out vec3 texcoord;"

			"void main() {"
				" texcoord = pos;"
				" gl_Position = (model*matrix*vec4(pos, 1.0)).xyww;"
			"}";
		
		const char* fragment_src = 
			"#version 330 core\n"
			"uniform samplerCube skybox;"
			"in vec3 texcoord;"
			"void main() {"
				" gl_FragColor = texture(skybox, texcoord);"
			"}";
		
		m_skybox.shader.load_from_memory(vertex_src, fragment_src);
		return true;
	}

	void Engine::unload_skybox() {
		if(m_skybox.shape.is_loaded()) {
			m_skybox.shape.unload();
		}
		if(m_skybox.texture.is_loaded()) { 
			m_skybox.texture.unload();
		}
	}
	
	void Engine::lock_mouse(const bool b) {
		SDL_SetWindowGrab(m_window, b ? SDL_TRUE : SDL_FALSE);
		SDL_SetRelativeMouseMode(b ? SDL_TRUE : SDL_FALSE);
		SDL_ShowCursor(!b);
		SDL_WarpMouseInWindow(m_window, m_width/2, m_height/2);
		if(b) {
			m_state.set(EngineState::LOCK_MOUSE);
		}
		else {
			m_state.unset(EngineState::LOCK_MOUSE);
		}

	}

	void Engine::vsync(const bool b) {
		if(SDL_GL_SetSwapInterval(b)) {
			message(MType::ERROR, "Error with VSync! %s", SDL_GetError());
		}
		if(b) {
			m_state.set(EngineState::VSYNC_ENABLED);
		}
		else {
			m_state.unset(EngineState::VSYNC_ENABLED);
		}
	}

	void Engine::fullscreen(const bool b) {
		SDL_SetWindowFullscreen(m_window, b ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
		SDL_GetWindowSize(m_window, &m_width, &m_height);
		glViewport(0, 0, m_width, m_height);
		if(m_cam != nullptr) {
			m_cam->aspect_ratio = get_aratio();
		}
		if(b) {
			m_state.set(EngineState::FULLSCREEN_ENABLED);
		}
		else {
			m_state.unset(EngineState::FULLSCREEN_ENABLED);
		}
	}

	void Engine::_needs_render_back(const bool b) {
		b ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	
	void Engine::winding_order(const int t_order) {
		glFrontFace((t_order <= 1) ? GL_CW : GL_CCW);
	}

	void Engine::setup_shaders(const std::vector<Shader*>& t_shaders) {
		for(size_t i = 0; i < t_shaders.size(); i++) {
			if(!t_shaders[i]->is_loaded()) { continue; }
			const uint32_t block_index = glGetUniformBlockIndex(t_shaders[i]->id, "vertex_data");
			glUniformBlockBinding(t_shaders[i]->id, block_index, 0);
		}

		// camera view, projection and position
		const uint32_t size = sizeof(glm::mat4) + sizeof(glm::vec3);

		glGenBuffers(1, &m_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo, 0, size);
	}

}


