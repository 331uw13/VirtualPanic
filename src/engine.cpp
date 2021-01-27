#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.hpp"
#include "messages.hpp"
#include "vbo.hpp"
#include "ibo.hpp"

namespace VPanic {
	
	Engine::Engine() {}
	Engine::~Engine() { quit(); }


	void Engine::set_update_callback(void(*t_callback)()) {
		m_update_callback = t_callback;
	}
	
	void Engine::init(const char* t_title, const Vec& t_size) {
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

		m_window = SDL_CreateWindow(t_title, 0, 0, t_size.x, t_size.y,
			   	SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);	
		
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
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// easier to me to think that (0, 0) is up left and (width, height) is down right
		//glOrtho(0, m_width, m_height, 0, 1, -1);
		
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		
		glEnable(GL_DEPTH_TEST); // z

		message(MType::DEBUG, "%s", glGetString(GL_VERSION));
		message(MType::DEBUG, "GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	

		const char* vertex_src =
			"#version 130\n"
			"in vec3 pos;"
			"uniform mat4 model;"
			"uniform mat4 view;"
			"uniform mat4 proj;"
			"void main() {"
			"gl_Position = proj*view*model*vec4(pos, 3);"
			"}";

		const char* fragment_src =
			"#version 130\n"
			"uniform vec4 color;"
			"void main() {"
			"gl_FragColor = vec4(color.x/255.0, color.y/255.0, color.z/255.0, 1.0);"
			"}";
		
		if(_create_shader(vertex_src, fragment_src)) {
			message(MType::OK, "Created default shader");
		}
		else {
			message(MType::BAD, "Failed to create default shader");
			quit();
			return;
		}

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
		
		// clean shaders
		message(MType::INFO, "Deleting %i shaders", m_shaders.size());
		for(uint32_t i = 0; i < m_shaders.size(); i++) {
			glDeleteProgram(m_shaders[i]);
		}
		m_shaders.clear();

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

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		uint32_t vao;
		VBO vbo;
		IBO ibo;

		vbo.gen();
		ibo.gen();
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		float points[] = {
			-0.5, -0.5,
			 0.5, -0.5,
			 0.5,  0.5,
			-0.5,  0.5,
		};

		uint32_t indx[] = {
			0, 1, 2,
			3, 2, 0
		};

		vbo.data(points);
		ibo.data(indx);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
		glEnableVertexAttribArray(0);

		float z = -1.5f;
		float z_inc = 0.08f;

		SDL_Event event;

		Vec p_mpos;
		float aratio = static_cast<float>(m_width) / static_cast<float>(m_height);

		while(!m_quit) {
			glClearColor(
					background_color.r / 255.0f,
				   	background_color.g / 255.0f,
				   	background_color.b / 255.0f,
				   	1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			Vec mpos = Mouse::get_pos();
			int r = map(mpos.x, 0, m_width, 255, 50);
			int g = map(mpos.x, 0, m_width, 50, 255);
			int b = map(mpos.y, 0, m_height, 50, 255);

		
			glUseProgram(m_shaders[0]);
			glUniform4f(glGetUniformLocation(m_shaders[0], "color"), r, g, b, 255);

			float dx = mpos.x - p_mpos.x;
			float dy = mpos.y - p_mpos.y;

			clamp<float>(dy, -15.0f, 15.0f);
			clamp<float>(dx, -15.0f, 15.0f);

			model = glm::rotate(model, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
        	view = glm::translate(view, glm::vec3(0.0f, 0.0f, z));
        	proj = glm::perspective(glm::radians(50.0f), aratio, 0.1f, 100.0f);
			
			glUniformMatrix4fv(glGetUniformLocation(m_shaders[0], "model"), 1, GL_FALSE, &model[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(m_shaders[0], "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(m_shaders[0], "proj"), 1, GL_FALSE, &proj[0][0]);

			glDrawElements(GL_TRIANGLES, ibo.index_count(), GL_UNSIGNED_INT, nullptr);
		
			z = 0.0f;
			p_mpos = mpos;
	
			// handle events
			while(SDL_PollEvent(&event)) {
				switch(event.type) {
					case SDL_QUIT:
						quit();
						break;

					case SDL_MOUSEWHEEL:
						if(event.wheel.y < 0) {
							z-=z_inc;
						}
						else if(event.wheel.y > 0){
							z+=z_inc;
						}
						
						message(MType::DEBUG, "Z: %f", z);
						break;

					default: break;
				}
			}
			SDL_GL_SwapWindow(m_window);
		}
		m_loop = false;
	}

	bool Engine::ok() {
		return (m_init_ok && m_window != nullptr);
	}
	
	bool Engine::_create_shader(const char* t_vertex_src, const char* t_fragment_src) {	
		int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &t_vertex_src, NULL);
		glCompileShader(vertex_shader);
		
		// check if shader has been compiled succesfully
		if(!_shader_ok(vertex_shader)) { return false; }

		int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &t_fragment_src, NULL);
		glCompileShader(fragment_shader);
		
		// check if shader has been compiled succesfully
		if(!_shader_ok(fragment_shader)) { return false; }
	
		
		// create and link the shader program
		int program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);
		
		int linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if(linked == 0) {
			char msg[1024];
			glGetProgramInfoLog(program, 1024, NULL, msg);
			message(MType::BAD, "%1(Shader Program Link Error):%0 %s", msg);
			return false;
		}

		
		// ok, looks good now save the program and delete the shader objects because
		// they are no longer needed
		
		m_shaders.push_back(program);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return true;
	}
	
	bool Engine::_shader_ok(int t_id) {
		int iv = 0;
		glGetShaderiv(t_id, GL_COMPILE_STATUS, &iv);
		if(iv == 0) {
			char msg[1024];
			glGetShaderInfoLog(t_id, 1024, NULL, msg);
			message(MType::BAD, "%1(Shader Compile Error):%0 %s", msg);
			return false;
		}
		return true;
	}

}







