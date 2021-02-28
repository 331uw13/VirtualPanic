#pragma once
#include <vector>
#include "shader.hpp"

class ImFont;


namespace vpanic {

	class ShaderEditor {
	public:

		void update();
		void open(Shader* t_shader_ptr);

		void load_font(const char* t_filename, const float t_size);

	private:


		std::string m_source { "void main() {\n\n\tvec3 col = vec3(0.0f, 1.0f, 0.0f);\n\tgl_FragColor = vec4(col, 1.0f);\n\n}\n" };
		Shader* m_shader { nullptr };
		ImFont* m_editor_font { nullptr };

	};

}
