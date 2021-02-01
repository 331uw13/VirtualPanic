#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "ImGui/imgui.h"

#include "mouse.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "camera.hpp"
#include "keyboard.hpp"
#include "timer.hpp"
#include "settings.hpp"

namespace VPanic {

	class Engine {
	public:

		Engine();
		~Engine();
		
		/*struct MouseData {
			float x, y;
			uint8_t rel_x, rel_y;
		};*/

		// TODO: 
		// - report mouse move
		// - user can choose if engine should handle the camera movement or not
		// - mouse can be shown/hidden
		// - global settings that live in stack

		//void set_mouse_move_callback(void(*t_callback)(MouseData));
		void set_mouse_wheel_callback(void(*t_callback)(uint8_t));
		void set_update_callback(void(*t_callback)());
		void set_keydown_callback(void(*t_callback)(uint8_t));
		
		void set_camera(Camera* t_cam);

		void init(const char* t_title, const glm::vec2& t_size, const int t_settings = 0);
		void start();

		void request_shutdown(); // use this instead of 'quit' when wanting to quit from event callback
		void quit();

		// check engine status
		bool ok() const;
		
		glm::vec2 get_window_size() const;
		float get_aratio() const;

		void camera_active(const bool b);
		void vsync(const bool b);
		void fullscreen(const bool b);

		Color background_color   { Color(15, 20, 30) };
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
		void(*m_mouse_wheel_callback)(uint8_t)  { nullptr };
		void(*m_keydown_callback)(uint8_t)      { nullptr };
		
		Camera* m_cam { nullptr };

		bool m_using_camera  { false };
		bool m_using_imgui   { false };

	};
}




