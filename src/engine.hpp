#pragma once
#include <vector>
#include <SDL2/SDL.h>

#include "vec.hpp"
#include "mouse.hpp"
#include "color.hpp"
#include "utils.hpp"

namespace VPanic {

	class Engine {
	public:

		Engine();
		~Engine();
		
		// mousewheel_callback
		// keypress_callback
		void set_update_callback(void(*t_callback)());

		void init(const char* t_title, const Vec& t_size);
		void execute();
		void quit();

		bool ok();
		
		Color background_color   { Color(25, 23, 22) };
		Color cursor_color       { Color(240, 240, 240, 188) };

	private:

		std::vector<int> m_shaders;

		bool _create_shader(const char* t_vertex_src, const char* t_fragment_src);
		bool _shader_ok(int t_id);

		bool m_init_ok    { false };
		bool m_quit       { false };
		bool m_loop       { false };
	
		int m_width   { 0 };
		int m_height  { 0 };

		SDL_Window* m_window     { nullptr };
		SDL_GLContext m_context  { NULL };

		void(*m_update_callback)() { nullptr };

	};
}




