#include "texture.hpp"
#include "messages.hpp"
#include "utils.hpp"

namespace VPanic {

	void Texture::render(Engine* t_engine) {
		if(!m_is_loaded) { return; }
		if(t_engine == nullptr) { return; }
		// TODO: need to check if can render
		if(m_data == nullptr) { return; }

		SDL_Renderer* renderer = t_engine->get_renderer();
		if(renderer == nullptr) { return; }
	
		Space dest(m_pos, m_size.x * m_scale.x, m_size.y * m_scale.y);
		if(out_of_bounds(dest, t_engine->get_window_size())) { return; }
		
		if(m_colorize) {
			// call this in colorize() ?
			SDL_SetTextureColorMod(m_data, m_color.r, m_color.g, m_color.b);
			SDL_SetTextureAlphaMod(m_data, m_color.a);
		}

		SDL_Rect src {
		   	0, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)
	   	};

		/*SDL_Rect dst { 
			static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			static_cast<int>(m_size.x * m_scale.x), static_cast<int>(m_size.y * m_scale.y)
	   	};*/

		SDL_Point p { dest.w / 2, dest.h / 2 }; // TODO: options for this?
		SDL_RenderCopyEx(
				renderer,
			   	m_data,
			   	&src,
			   	&dest,
			   	m_angle,
			   	&p,
			   	SDL_FLIP_NONE
		);
	}

	void Texture::load(Engine* t_engine, const char* t_filepath) {
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

		// if size not set then set it to the texture size
		if(m_size <= vec2(0)) {
			int w = 0;
			int h = 0;
			SDL_QueryTexture(m_data, NULL, NULL, &w, &h);
			m_size = vec2(w, h);
		}

		// save original color
		SDL_GetTextureColorMod(m_data, 
				&m_origin_color.r,
				&m_origin_color.g,
				&m_origin_color.b);

		m_is_loaded = true;
	}

	void Texture::reload(Engine* t_engine, const char* t_new_filepath) {
		unload();
		load(t_engine, t_new_filepath);
	}

	void Texture::unload() {
		if(m_data == nullptr) { return; }
		SDL_DestroyTexture(m_data);
		m_data = nullptr;
		m_size = vec2(0);
		m_is_loaded = false;
	}

	bool Texture::is_loaded() {
		return m_is_loaded;
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

	Color Texture::get_color() {
		return m_color;
	}

	void Texture::colorize(const Color& t_color) {
		m_color = t_color;
		clamp<uint8_t>(m_color.r, 0, 255);
		clamp<uint8_t>(m_color.g, 0, 255);
		clamp<uint8_t>(m_color.b, 0, 255);
		m_colorize = true;
	}

	void Texture::default_color() {
		m_colorize = false;	
		SDL_SetTextureColorMod(m_data, m_origin_color.r, m_origin_color.g, m_origin_color.b);
		SDL_SetTextureAlphaMod(m_data, 255);
	}

	//void Texture::crop(const Space& t_area) {
	//}

	Space Texture::get_space() {
		return Space(m_pos, m_size.x * m_scale.x, m_size.y * m_scale.y);
	}

	vec2 Texture::get_scale() const {
		return m_scale;
	}

}


