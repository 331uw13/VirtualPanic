#pragma once
#include <vector>
#include <SDL2/SDL.h>

#include "timer.hpp"
#include "mouse.hpp"
#include "settings.hpp"
#include "color.hpp"

#include "imgui/imgui.h"

namespace VPanic {
	
	class Engine {
	public:

		void init(const char* t_title, const vec2& t_size, int t_settings);
		void execute();
		void quit();
		
		void show_cursor(bool t_visible);

		void set_update_callback(void(*t_callback)(int));
		void set_keypress_callback(void(*t_callback)(SDL_Keycode));
		void set_mousewheel_callback(void(*t_callback)(int));

		int get_fps() const;

		vec2 get_window_size();
		vec2 get_window_center();
		
		Color background_color   { Color(25, 23, 22) };
		Color cursor_color       { Color(240, 240, 240, 188) };

		Engine() {}
		~Engine() { quit(); }

	private:

		bool _can_render();

		void(*m_update_callback)(int)                { nullptr };		
		void(*m_keypress_callback)(SDL_Keycode)      { nullptr };
		void(*m_mousewheel_callback)(int)            { nullptr };

		// TODO: use array for these settings!

		bool m_cursor_visible  { true };
		bool m_renderer_fill   { true };
		bool m_using_imgui     { false };

		bool m_init_ok    { false };
		bool m_quit       { false };
		bool m_loop       { false };

		int m_fps          { 0 };
		
		vec2 m_win_size { vec2() };
		Timer m_timer   { Timer() };

		SDL_Window*    m_win        { nullptr };
		SDL_Renderer*  m_renderer   { nullptr };
	};
	
}


