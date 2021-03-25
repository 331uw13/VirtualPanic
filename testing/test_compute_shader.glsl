uniform int max_particles;
uniform float dt;
uniform vec4 origin;
uniform bool mouse_down;

struct Particle {
	vec4 attr0;
	vec4 attr1;
	vec4 attr2;
	vec3 attr3;
	float life;
};

layout (local_size_x = 512, local_size_y = 1, local_size_z = 1) in;
layout (std430, binding = 0) buffer particles {
	Particle particle_buffer[];
};

uint rng_state;

uint rand_lcg() {
    // LCG values from Numerical Recipes
    rng_state = 1664525 * rng_state + 1013904223;
    return rng_state;
}

uint rand_xorshift() {
    // Xorshift algorithm from George Marsaglia's paper
    rng_state ^= (rng_state << 13);
    rng_state ^= (rng_state >> 17);
    rng_state ^= (rng_state << 5);
    return rng_state;
}

float rand() {
	return float(rand_xorshift()) * (1.0 / 4294967296.0);
}


void main() {

	const uint id = gl_GlobalInvocationID.x;
	if(id >= max_particles) { return; }
	rng_state = id;
	
	Particle p = particle_buffer[id];

	if(p.life <= 0.0f || mouse_down) {
		p.life = 1.0f;

		float x = rand()-0.5f;
		float y = rand()-0.5f;
		float z = rand()-0.5f;

		vec4 n = vec4(x, y, z, 1.0f);
		p.attr0 = origin+n*rand();	
	}

	p.attr1 += 1.99f*normalize(origin-p.attr0)*length(p.attr0)-0.09f;
	p.attr0 += p.attr1*dt*dt;
	
	p.attr2 = vec4(1.0f, 0.05f, 0.0f, 0.6f);
	particle_buffer[id] = p;
}

