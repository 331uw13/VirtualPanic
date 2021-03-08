#pragma once
#include <vector>
#include <SDL2/SDL.h>
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
#include "state.hpp"


namespace vpanic {

	class Engine {
	public:

		Engine();
		~Engine();
	
		void mouse_move_callback(void(*t_callback)(const MouseData&));  // when mouse is moving
		void mouse_wheel_callback(void(*t_callback)(int8_t));    // when mouse wheel is turned up or down
		void keydown_callback(void(*t_callback)(uint8_t));       // when any keyboard key is down but not good for movement
		void update_callback(void(*t_callback)());               // every frame

		void init(const char* t_title, const Vec2& t_size, const int t_settings = 0);
		void start();

		void request_shutdown(); // use this instead of 'quit' when wanting to quit from event callback
		void quit();
	

		Vec2 window_size() const;
		float aspect_ratio() const;
		float delta_time() const;

		// it needs to know what shaders wants to know camera matrix
		void setup_shaders(const std::vector<Shader*>& t_shaders);

		void use_camera(Camera* t_cam_ptr);
		bool load_skybox(const std::vector<const char*> t_files);
		void unload_skybox();
		void lock_mouse(const bool b);
		void vsync(const bool b);
		void fullscreen(const bool b);
		void winding_order(const int t_order); // vpanic::CLOCKWISE, vpanic::COUNTER_CLOCKWISE
		void mouse_to_window_center();

		EngineState copy_state() const;
		EngineState& get_state_ref();

		Color background_color   { Color(0, 0, 0) };

	private:
		
		EngineState m_state;

		struct Skybox {
		
			Texture texture;
			Shape shape;
			Shader shader;

		} m_skybox;

		uint32_t m_ubo { 0 };  // "uniform buffer object"
		
		int m_width    { 0 };
		int m_height   { 0 };
		float m_delta_time { 0.0f };

		void(*m_update_callback)()              { nullptr };
		void(*m_keydown_callback)(uint8_t)      { nullptr };
		void(*m_mouse_wheel_callback)(int8_t)   { nullptr };
		void(*m_mouse_move_callback)(const MouseData&) { nullptr };
		
		void _update_engine_ok_state();
		void _needs_render_back(const bool b);

		Camera* m_cam            { nullptr };
		SDL_Window* m_window     { nullptr };
		SDL_GLContext m_context  { NULL };

	};
}





