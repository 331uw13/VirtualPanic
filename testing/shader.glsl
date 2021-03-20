uniform vec3 light_pos;
uniform vec4 light_color;

uniform float light_radius;
uniform float light_brightness;
uniform vec3 particle_origin;


void main() {
	//gl_FragColor = vec4(vpanic_directional_light(vec3(0.45f, 1.2f, 2.65f), light_color, 2.14f, 0.42f, 0.21f), 1.0);
	
	vec4 col = vec4(0.0f);
	col += vec4(vpanic_light(particle_origin, vec4(0.0f, 1.0f, 1.0f, 1.0f), 40.f, 2.5f), 1.0);
	col += vec4(vpanic_light(light_pos, light_color, light_radius, light_brightness), 1.0);

	gl_FragColor = col;
}
