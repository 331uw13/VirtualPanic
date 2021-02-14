#pragma once
#include <glm/vec3.hpp>

#include "color.hpp"

// NOTE: this is useless, delete this

namespace vpanic {

	struct Light {

		glm::vec3 pos;
		Color color;	
		float brightness { 1.0f };
		float radius { 10.f };

		Light(const glm::vec3& t_pos, const Color& t_color)
			: pos(t_pos), color(t_color) {}

		Light()
		   	: pos(glm::vec3(0.0f)), color(Color()) {}

	};


}
