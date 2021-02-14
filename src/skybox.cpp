#include "skybox.hpp"
#include "messages.hpp"

#include "libs/stb_image.h"

namespace vpanic {

	bool Skybox::load(const std::vector<std::string> t_filenames) {
	
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

		int width = 0;
		int height = 0;
		int num_channels = 0;

		message(MType::INFO, "Loading skybox...");

		for(uint32_t i = 0; i < t_filenames.size(); i++) {
			uint8_t* data = stbi_load(t_filenames[i].c_str(), &width, &height, &num_channels, 0);
			
			if(!data) {
				message(MType::BAD, "Cannot load texture from file: \"%s\"", t_filenames[i].c_str());
				stbi_image_free(data);
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

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, channel, width, height, 0, channel,
				   	GL_UNSIGNED_BYTE, data);
			
			stbi_image_free(data);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
		}
		
		message(MType::OK, "Loaded skybox %i images", t_filenames.size());
		
		m_box.load({
				
			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f, -0.5f),
			Vertex( 0.5f,  0.5f, -0.5f),
			Vertex( 0.5f,  0.5f, -0.5f),
			Vertex(-0.5f,  0.5f, -0.5f),
			Vertex(-0.5f, -0.5f, -0.5f),

			Vertex(-0.5f, -0.5f,  0.5f),
			Vertex( 0.5f,  0.5f,  0.5f),
			Vertex( 0.5f, -0.5f,  0.5f),
			Vertex( 0.5f,  0.5f,  0.5f),
			Vertex(-0.5f, -0.5f,  0.5f),
			Vertex(-0.5f,  0.5f,  0.5f),

			Vertex(-0.5f,  0.5f,  0.5f),
			Vertex(-0.5f, -0.5f, -0.5f), 
			Vertex(-0.5f,  0.5f, -0.5f), 
			Vertex(-0.5f, -0.5f, -0.5f), 
			Vertex(-0.5f,  0.5f,  0.5f), 
			Vertex(-0.5f, -0.5f,  0.5f), 

			Vertex( 0.5f,  0.5f,  0.5f),
			Vertex( 0.5f,  0.5f, -0.5f),
			Vertex( 0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f,  0.5f),
			Vertex( 0.5f,  0.5f,  0.5f),

			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f,  0.5f),
			Vertex( 0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f,  0.5f),
			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex(-0.5f, -0.5f,  0.5f),
		
			Vertex(-0.5f,  0.5f, -0.5f),
			Vertex( 0.5f,  0.5f, -0.5f),
			Vertex( 0.5f,  0.5f,  0.5f),
			Vertex( 0.5f,  0.5f,  0.5f),
			Vertex(-0.5f,  0.5f,  0.5f),
			Vertex(-0.5f,  0.5f, -0.5f)

		}, NO_COLORED_LIGHT);

		return true;
	}	
	
	void Skybox::draw(const Shader& t_shader, const Camera& t_cam) {
	
		// TODO: optimize this
		
		glDepthMask(GL_FALSE);
		t_shader.use();
		t_shader.set_mat4("view", glm::mat4(glm::mat3(t_cam.view))); // removes translation
		m_box.draw(t_shader);
		glDepthMask(GL_TRUE);
	}

}







