#include "libs/gl3w.h"

#include "shader_component.hpp"
#include "messages.hpp"


namespace vpanic {

	void ShaderComponent::compile(const int t_type) {
		if(src.length() <= 1) { 
			message(MType::ERROR, "ShaderComponent source is empty");
			return;
		};

		type = t_type;
		const int gltype = [t_type]() {
			switch(t_type) {
				case VERTEX_SHADER:    return GL_VERTEX_SHADER;
				case FRAGMENT_SHADER:  return GL_FRAGMENT_SHADER;
				case GEOMETRY_SHADER:  return GL_GEOMETRY_SHADER;
				case COMPUTE_SHADER:   return GL_COMPUTE_SHADER;
				default:               return -1;
			}
			return -1;
		}();

		if(gltype < 0) {
			type = INVALID_SHADER;
			return;
		}
		
		id = glCreateShader(gltype);
		const char* c = src.c_str();
		glShaderSource(id, 1, &c, NULL);
		glCompileShader(id);
	}

}
