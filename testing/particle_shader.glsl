uniform vec3 light_pos;
uniform vec4 light_color;

const float light_radius = 8.f;
const float light_brightness = 1.0f;

/*
layout(std430, binding=0) buffer testing {
	vec3 particle_pos;
};
*/
void main() {

	gl_FragColor = vec4(0.0f, 0.0f, 1.0f, 0.4f);

	//gl_FragColor = vec4(vpanic_directional_light(vec3(1.5f, 3.0f, 2.65f), light_color, 2.14f, 0.42f, 0.21f), shape.color.w);
	//gl_FragColor = vec4(vpanic_light(light_pos, lightcolor, light_radius, light_brightness), 1.0);
}



