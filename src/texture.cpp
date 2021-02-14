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

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, num_channels;

		stbi_set_flip_vertically_on_load(true);
		uint8_t* data = stbi_load(t_filename, &width, &height, &num_channels, 0);
		if(!data) {
			message(MType::BAD, "Cannot load texture from file: \"%s\" | %s", t_filename, stbi_failure_reason());
			return false;
		}

		const uint32_t channel = [num_channels]() {
			switch(num_channels) {
				case 2: return GL_RG;
				case 3: return GL_RGB;
				case 4: return GL_RGBA;
				default: return GL_RGB;
			}
		}();
		glTexImage2D(GL_TEXTURE_2D, 0, channel, width, height, 0, channel, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		message(MType::OK, "Loaded texture %3\"%s\"", t_filename);
		m_loaded = true;
		return true;
	}

	void Texture::unload() {
		message(MType::INFO, "Unload texture");
		glDeleteTextures(1, &m_id);
		m_id = 0;
		m_loaded = false;
	}
	
	bool Texture::is_loaded() const {
		return m_loaded;
	}

	void Texture::use() {
		if(!m_loaded) { return; }
		glBindTexture(GL_TEXTURE_2D, m_id);
	}


}
