#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "engine.hpp"
#include "space.hpp"

namespace VPanic {

	class Texture {
	public:

		void render(const Engine* t_engine);
		void load(const Engine* t_engine, const char* t_filepath);
		void reload(const Engine* t_engine, const char* t_new_filepath);
		void unload();

		void position(const vec2& t_pos);
		void scale(const vec2& t_scale);
		void rotate(float t_angle);
		void colorize(const Color& t_color);
		void crop(const Space& t_area);

		Space get_space();

		Texture() {}
		~Texture() { unload(); }

	private:

		SDL_Texture* m_data { nullptr };

		vec2 m_scale  { vec2(1) };
		vec2 m_pos    { vec2() };
		Color m_color { Color(0, 0, 0, 0) };
		Space m_area  { Space() };
		float m_angle { 0.0f };
		
	};

}
