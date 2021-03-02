#include <GL/gl3w.h>

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include "texture.hpp"
#include "messages.hpp"

namespace vpanic {

	Texture::Texture(const char* t_filename) {
		load(t_filename);
	}

	Texture::~Texture() {
		unload();
	}

	bool Texture::load(const char* t_filename) {
		if(m_loaded) { return false; }

		m_type = GL_TEXTURE_2D;

		glGenTextures(1, &m_id);
		glBindTexture(m_type, m_id);

		glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		stbi_set_flip_vertically_on_load(true);
		return _generate_image(t_filename, m_type);
	}
	
	bool Texture::load_cube(const std::vector<const char*>& t_filenames) {
		if(m_loaded) { return false; }
		

		m_type = GL_TEXTURE_CUBE_MAP;

		glGenTextures(1, &m_id);
		glBindTexture(m_type, m_id);

		for(uint32_t i = 0; i < t_filenames.size(); i++) {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

			if(!_generate_image(t_filenames[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X+i)) {
				return false;
			}
		}
		return true;
	}
	
	bool Texture::load_cube(const char* t_filename) {
		return load_cube({ t_filename, t_filename, t_filename, t_filename, t_filename, t_filename }); // lol
	}

	bool Texture::_generate_image(const char* t_filename, const int t_type) {
		int width = 0;
		int height = 0;
		int num_channels = 0;
		uint8_t* data = nullptr;

		data = stbi_load(t_filename, &width, &height, &num_channels, 0);
		if(data == nullptr) {
			message(MType::ERROR, "Cannot load texture from file: \"%s\" | reason: \"%s\"", t_filename, stbi_failure_reason());
			stbi_image_free(data);
			return true;
		}

		const uint32_t channel = [num_channels]() {
			switch(num_channels) {
				case 2: return GL_RG;
				case 3: return GL_RGB;
				case 4: return GL_RGBA;
				default: return GL_RGB;
			}
		}();

		glTexImage2D(t_type, 0, channel, width, height, 0, channel, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		
		glGenerateMipmap(m_type);
		
		message(MType::OK, "Loaded texture: \"%s\"", t_filename);
		m_loaded = true;
		return true;
	}

	void Texture::unload() {
		if(!m_loaded) { return; }
		glDeleteTextures(1, &m_id);
		m_id = 0;
		m_loaded = false;
	}
	
	bool Texture::is_loaded() const {
		return m_loaded;
	}
	
	uint8_t Texture::get_type() const {
		return m_type;
	}

	void Texture::enable() {
		if(!m_loaded) { return; }
		glBindTexture(m_type, m_id);
	}
	
	void Texture::disable() {
		glBindTexture(m_type, 0);
	}


}
