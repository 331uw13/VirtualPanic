uniform vec3 light_pos;
uniform vec4 light_color;


out vec4 out_color;
void main() {

	vec4 o = vec4(0.0f);
	o += vec4(vpanic_light(light_pos, light_color, 10.f), 1.0f);

	out_color = o;
}

