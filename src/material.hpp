#pragma once
#include "vec.hpp"

namespace VPanic {

	struct Material {
	
		vec3 ambient   { vec3(0.0f) };
		vec3 diffuse   { vec3(0.0f) };
		vec3 specular  { vec3(0.0f) };
		float shine    { 0.0f };

		Material(const vec3& t_ambient, const vec3& t_diffuse, const vec3& t_specular, const float t_shine)
			: ambient(t_ambient), diffuse(t_diffuse), specular(t_specular), shine(t_shine) {}

		Material() {}

	};

}
