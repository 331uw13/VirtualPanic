#pragma once
#include <vector>
#include <SDL2/SDL.h>

#include "vec.hpp"
#include "mouse.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "camera.hpp"
#include "keyboard.hpp"
#include "timer.hpp"

namespace VPanic {

	class Engine {
	public:

		Engine();
		~Engine();
		
		void set_update_callback(void(*t_callback)());
		void set_mousewheel_callback(void(*t_callback)(uint8_t));
		void set_keydown_callback(void(*t_callback)(uint8_t));
		
		void set_camera(Camera* t_cam);

		void init(const char* t_title, const vec2& t_size);
		void execute();
		void quit();
		bool ok();
		
		vec2 get_window_size() const;
		vec2 get_window_center() const;
		float get_aratio() const;

		void fullscreen(const bool b);

		Color background_color   { Color(5, 3, 2) };
		Color cursor_color       { Color(240, 240, 240, 188) };
		
	private:

		bool m_init_ok    { false };
		bool m_quit       { false };
		bool m_loop       { false };
	
		int m_width   { 0 };
		int m_height  { 0 };

		SDL_Window* m_window     { nullptr };
		SDL_GLContext m_context  { NULL };

		void(*m_update_callback)() { nullptr };
		void(*m_mousewheel_callback)(uint8_t) { nullptr };
		void(*m_keydown_callback)(uint8_t) { nullptr };
		
		Camera* m_cam { nullptr };

	};
}




