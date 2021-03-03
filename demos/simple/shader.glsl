uniform vec3 light_pos;
uniform vec4 light_color;

const float light_radius = 8.f;
const float light_brightness = 1.0f;

void main() {
	gl_FragColor = vec4(vpanic_light(light_pos, light_color, light_radius, light_brightness), 1.0);
}
