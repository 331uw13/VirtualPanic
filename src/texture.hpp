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
		
		//void crop(const Space& t_area);

		void colorize(const Color& t_color);
		void default_color();
		
		Color get_color();
		Space get_space();

		vec2 pos;
		vec2 scale;
		float angle;

		Texture() : pos(vec2()), scale(vec2(1)), angle(0.0f) {}
		~Texture() {}

	private:

		SDL_Texture* m_data   { nullptr };
		bool m_is_loaded      { false };
		bool m_colorize       { false };
		Color m_color         { Color() };
		Color m_origin_color  { Color() };
		vec2 m_size           { vec2() };

	};

}
