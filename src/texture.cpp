#include "texture.hpp"
#include "messages.hpp"
#include "utils.hpp"

namespace VPanic {

	void Texture::render(const Engine* t_engine) {
		if(t_engine == nullptr) { return; }
		if(m_data == nullptr) { return; }

		SDL_Renderer* renderer = t_engine->get_renderer();
		if(renderer == nullptr) { return; }

		// TODO: dont render it if its out of bounds!

		if(m_color.a > 0) {
			SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
		}

		SDL_Rect src {
		   	m_area.x, m_area.y, m_area.w, m_area.h
	   	};

		SDL_Rect dst { 
			static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			static_cast<int>(m_area.w * m_scale.x), static_cast<int>(m_area.h * m_scale.y)
	   	};

		SDL_Point p { 0, 0 }; // todo
		SDL_RenderCopyEx(
				renderer,
			   	m_data,
			   	&src,
			   	&dst,
			   	0,
			   	&p,
			   	SDL_FLIP_NONE
		);
	}

	void Texture::load(const Engine* t_engine, const char* t_filepath) {
		if(m_data != nullptr) { 
			message("Texture is already loaded, try reload!", MsgType::WARNING);
			return;
	   	}
		SDL_Surface* s = IMG_Load(t_filepath);
		if(s == nullptr) {
			message(IMG_GetError(), MsgType::ERROR);
			return;
		}
		m_data = SDL_CreateTextureFromSurface(t_engine->get_renderer(), s);
		if(m_data == nullptr) {
			message(SDL_GetError(), MsgType::ERROR);
			return;
		}
		SDL_FreeSurface(s);
		message("Loaded", t_filepath, MsgType::DEBUG);

		// if the area is not set then set it to the texture size
		if(m_area.empty()) {
			int w = 0;
			int h = 0;
			SDL_QueryTexture(m_data, NULL, NULL, &w, &h);
			m_area = Space(0, 0, w, h);
		}
	}

	void Texture::reload(const Engine* t_engine, const char* t_new_filepath) {
		unload();
		load(t_engine, t_new_filepath);
	}

	void Texture::unload() {
		if(m_data == nullptr) { return; }
		SDL_DestroyTexture(m_data);
		m_data = nullptr;
		m_area = Space();
		message("Unloaded Texture!", MsgType::DEBUG);
	}


	void Texture::position(const vec2& t_pos) {
		m_pos = t_pos;
	}

	void Texture::scale(const vec2& t_scale) {
		m_scale = t_scale;
	}

	void Texture::rotate(float t_angle) {
		m_angle = t_angle;
	}

	void Texture::colorize(const Color& t_color) {
		m_color = t_color;
	}

	void Texture::crop(const Space& t_area) {
		m_area = t_area;
	}

	Space Texture::get_space() {
		return Space(m_pos.x, m_pos.y, m_area.w, m_area.h);
	}

}


