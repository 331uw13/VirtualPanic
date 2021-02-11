#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "ImGui/imgui.h"

#include "mouse.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "keyboard.hpp"
#include "timer.hpp"
#include "settings.hpp"


namespace VPanic {

	class Engine {
	public:

		Engine();
		~Engine();
	
		// set them 'nullptr' if you want to disable them
		void mouse_move_callback(void(*t_callback)(const MouseData&));  // when mouse is moving
		void mouse_wheel_callback(void(*t_callback)(uint8_t));    // when mouse wheel is turned up or down
		void keydown_callback(void(*t_callback)(uint8_t));        // when any keyboard key is down but not good for movement
		void update_callback(void(*t_callback)());                // every frame
		
		void init(const char* t_title, const glm::vec2& t_size, const int t_settings = 0);
		void start();

		void request_shutdown(); // use this instead of 'quit' when wanting to quit from event callback
		void quit();

		// check engine status
		bool ok() const;
		
		glm::vec2 get_window_size() const;
		float get_aratio() const;

		void lock_mouse(const bool b);
		void vsync(const bool b);
		void fullscreen(const bool b);
		void render_back(const bool b);
		void winding_order(const int t_order); // VPanic::CLOCKWISE, VPanic::COUNTER_CLOCKWISE

		Color background_color   { Color(0, 0, 0) };
		// TODO: Color cursor_color       { Color(240, 240, 240, 188) };
		
	private:

		bool m_init_ok    { false };
		bool m_quit       { false };
		bool m_loop       { false };

		int m_width   { 0 };
		int m_height  { 0 };

		SDL_Window* m_window     { nullptr };
		SDL_GLContext m_context  { NULL };

		void(*m_update_callback)()              { nullptr };
		void(*m_keydown_callback)(uint8_t)      { nullptr };
		void(*m_mouse_wheel_callback)(uint8_t)  { nullptr };
		void(*m_mouse_move_callback)(const MouseData&) { nullptr };

		bool m_lock_mouse    { false };
		bool m_using_imgui   { false };

	};
}




