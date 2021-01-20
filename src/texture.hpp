#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "engine.hpp"
#include "space.hpp"

namespace VPanic {

	class Texture {
	public:

		void render(Engine* t_engine);
		void load(Engine* t_engine, const char* t_filepath);
		void reload(Engine* t_engine, const char* t_new_filepath);
		void unload();

		bool is_loaded();
		
		void position(const vec2& t_pos);
		void scale(const vec2& t_scale);
		void rotate(float t_angle);
		//void crop(const Space& t_area);

		void colorize(const Color& t_color);
		void default_color();
		
		Color get_color();
		Space get_space();

		Texture() {}
		~Texture() {}

	private:

		SDL_Texture* m_data { nullptr };
		bool m_is_loaded { false };

		vec2 m_pos    { vec2() };
		vec2 m_size   { vec2() };
		vec2 m_scale  { vec2(1) };
		float m_angle { 0.0f };
		
		bool m_colorize       { false };
		Color m_color         { Color() };
		Color m_origin_color  { Color() };

	};

}
