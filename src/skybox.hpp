#pragma once
#include <vector>
#include <string>

#include "shape.hpp"
#include "camera.hpp"

namespace vpanic {

	class Skybox {
	public:

		bool load(const std::vector<std::string> t_filenames);
		void draw(const Shader& t_shader, const Camera& t_cam);

	private:

		Shape m_box;
		uint32_t m_id { 0 };
	
	};

}
