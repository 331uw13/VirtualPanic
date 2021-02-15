#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "libs/imgui/imgui.h"

#include "mouse.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "keyboard.hpp"
#include "timer.hpp"
#include "settings.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "shape.hpp"


namespace vpanic {

	class Engine {
	public:

		Engine();
		~Engine();
	
		// set them 'nullptr' if you want to disable them
		void mouse_move_callback(void(*t_callback)(const MouseData&));  // when mouse is moving
		void mouse_wheel_callback(void(*t_callback)(int8_t));    // when mouse wheel is turned up or down
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
		
		Camera camera; // <-- temporary solution!

		void setup_shaders(const std::vector<Shader*>& t_shaders);
		bool load_skybox(const std::vector<const char*> t_files);
		void unload_skybox();
		void lock_mouse(const bool b);
		void vsync(const bool b);
		void fullscreen(const bool b);
		void render_back(const bool b); // do you want to see the backside of a shape or not?
		void winding_order(const int t_order); // VPanic::CLOCKWISE, VPanic::COUNTER_CLOCKWISE
		
		Color background_color   { Color(0, 0, 0) };

	private:

/*
		enum {
			INIT_OK = 0,
			QUIT = 1,
			KEEP_LOOP = 2,
			LOCK_MOUSE = 3,
			USING_IMGUI = 4,
		};

		bool m_switches[5];
*/

		struct Skybox {
		
			uint32_t id  { 0 };
			bool loaded  { false };
			Shape shape;
			Shader shader;

		} m_skybox;


		uint32_t m_ubo { 0 };  // "uniform buffer object"
		
		bool m_init_ok    { false };
		bool m_quit       { false };
		bool m_loop       { false };
		
		bool m_lock_mouse    { false };
		bool m_using_imgui   { false };

		int m_width    { 0 };
		int m_height   { 0 };

		void(*m_update_callback)()              { nullptr };
		void(*m_keydown_callback)(uint8_t)      { nullptr };
		void(*m_mouse_wheel_callback)(int8_t)  { nullptr };
		void(*m_mouse_move_callback)(const MouseData&) { nullptr };
		
		SDL_Window* m_window     { nullptr };
		SDL_GLContext m_context  { NULL };

	};
}





