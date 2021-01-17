#include <random>
#include <time.h>
#include <math.h>
#include <climits>
#include <SDL2/SDL_image.h>

#include "imgui/imgui_sdl/imgui_sdl.h"

#include "engine.hpp"
#include "messages.hpp"
#include "utils.hpp"

namespace VPanic {
	
	void Engine::show_cursor(bool t_visible) {
		m_cursor_visible = t_visible;
	}
	
	void Engine::init(const char* t_title, const vec2& t_size, int t_settings) {
		if(m_init_ok) {
			message("Engine is already started!", MsgType::ERROR);
		}

		// Initialize SDL and few things
		
		message("Initializing engine");
	
		set_seed(time(0));
		
		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			message("Error occurred while trying to initialize SDL", MsgType::ERROR);
			message(SDL_GetError(), MsgType::ERROR);
			return;
		}

		unsigned int win_flags = SDL_WINDOW_SHOWN;
		unsigned int render_flags = SDL_RENDERER_ACCELERATED;
		
		if(t_settings & FULLSCREEN) {
			win_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			message("Using Fullscreen", MsgType::DEBUG);
		}
		if(t_settings & USE_VSYNC) {
			render_flags |= SDL_RENDERER_PRESENTVSYNC;
			message("Using VSync", MsgType::DEBUG);
		}
		if(t_settings & USE_IMGUI) {
			m_using_imgui = true;
			message("Using ImGui & ImGuiSDL", MsgType::DEBUG);
		}
		
		m_win = SDL_CreateWindow(t_title, 0, 0, t_size.x, t_size.y, win_flags);
			
		if(m_win == nullptr) {
			message("Error occurred while trying to create new window", MsgType::ERROR);
			message(SDL_GetError(), MsgType::ERROR);
			return;	
		}
		message("Created window");
		
		int w = 0;
		int h = 0;
		SDL_GetWindowSize(m_win, &w, &h);

		m_win_size.x = static_cast<float>(w);
		m_win_size.y = static_cast<float>(h);

		m_renderer = SDL_CreateRenderer(m_win, -1, render_flags);

		if(m_renderer == nullptr) {
			message("Error occurred while trying to create renderer", MsgType::ERROR);
			message(SDL_GetError(), MsgType::ERROR);
			return;	
		}
	
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

		message("Created Renderer");
		
		if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
			message("Error occurred while trying to initialize SDL_image", MsgType::ERROR);
			message(SDL_GetError(), MsgType::ERROR);
			return;	
		}
		message("Initialized SDL_image");
		message("Success! init ok.", MsgType::SUCCESS);
		
		m_init_ok = true;
		
		if(m_update_callback == nullptr) {
			message("Update callback is not set!", MsgType::WARNING);
		}

		SDL_ShowCursor(false);
		// ? SDL_RenderSetScale(m_renderer, m_pixel_size, m_pixel_size);
	
		if(m_using_imgui) {
			ImGui::CreateContext();
			ImGuiSDL::Initialize(m_renderer, m_win_size.x, m_win_size.y);
		}
	
	}

	void Engine::quit() {
		if(!m_init_ok) { return; }
		
		message("Cleaning...");

		if(m_win != nullptr) { 
			SDL_DestroyWindow(m_win);
	   		m_win = nullptr;
			message("Window is destroyed");
		}
		if(m_renderer != nullptr) {
			SDL_DestroyRenderer(m_renderer);
			m_renderer = nullptr;
			message("Renderer is destroyed");
		}
		SDL_Quit();

		if(m_using_imgui) {
			ImGuiSDL::Deinitialize();
			ImGui::DestroyContext();
			message("ImGui context is destroyed");
		}

		m_quit = true;
		m_init_ok = false;
		message("Engine cleaning done!");
	}

	void Engine::set_update_callback(void(*t_callback)(int)) {
		if(t_callback != nullptr) {
			m_update_callback = t_callback;
		}
	}

	void Engine::set_keypress_callback(void(*t_callback)(SDL_Keycode)) {
		if(t_callback != nullptr) {
			m_keypress_callback = t_callback;
		}
	}
	
	void Engine::set_mousewheel_callback(void(*t_callback)(int)) {
		if(t_callback != nullptr) {
			m_mousewheel_callback = t_callback;
		}
	}

	int Engine::get_fps() const {
		return m_fps;
	}

	vec2 Engine::get_window_size() const {
		return m_win_size;
	}

	vec2 Engine::get_center() {
		return m_win_size / 2;
	}

	void Engine::execute() {
		if(!_can_render()) {
			message("Engine is not initialized", MsgType::ERROR);
			return;
		}
		if(m_loop) { return; }
		m_loop = true;
		
		SDL_Event event;
		while(!m_quit) {
			int start_tick = SDL_GetTicks();
			while(SDL_PollEvent(&event)) {
				if(m_using_imgui) {
					ImGuiSDL::ProcessEvent(&event);
				}
				switch(event.type) {
					case SDL_QUIT:
						m_quit = true;
						break;

					case SDL_KEYDOWN:
						if(m_keypress_callback != nullptr) {
							m_keypress_callback(event.key.keysym.sym);
						}
						break;

					case SDL_MOUSEWHEEL:
						if(m_mousewheel_callback != nullptr) {
							m_mousewheel_callback(event.wheel.y);
						}
						break;

					default: break;
				}
			}

			if(m_using_imgui) {
				ImGui::NewFrame();
			}

			SDL_SetRenderDrawColor
				(m_renderer, background_color.r, background_color.g, background_color.b, background_color.a);
			
			SDL_RenderClear(m_renderer);
			
			if(m_update_callback != nullptr) {
				m_update_callback(m_timer.elapsed());
			}

			if(m_using_imgui) {
				ImGui::Render();
				ImGuiSDL::Render(ImGui::GetDrawData());
			}
			
			if(m_cursor_visible) {
				// draw cursor
				// TODO: support texture!
				render(Space(Mouse::get_pos(), 14, 4.4f), cursor_color);
				render(Space(Mouse::get_pos(), 4.4f, 14), cursor_color);
			}
		
			// finally update the screen
			SDL_RenderPresent(m_renderer);

			m_timer.reset();
			
			int elapsed = (SDL_GetTicks() - start_tick);
			if((1000 / 60) > elapsed && !m_quit) {
				SDL_Delay(1000 / 60 - elapsed);
			}
			m_fps = (elapsed > 0) ? 1000 / elapsed : 0;
			
		}
		m_loop = false;
	}

	bool Engine::_can_render() {
		return (m_win != nullptr && m_renderer != nullptr && m_init_ok);
	}
}







