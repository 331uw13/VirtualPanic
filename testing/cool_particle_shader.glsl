uniform int max_particles;
uniform float dt;
uniform vec4 origin;

struct Particle {
	vec4 pos;
	vec4 velocity;
};


layout (local_size_x = 512, local_size_y = 1, local_size_z = 1) in;
layout (std430, binding = 0) buffer particles {
	Particle particle_array[];
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

	rand_lcg();

	const uint id = gl_GlobalInvocationID.x;
	if(id <= max_particles) {
		Particle p = particle_array[id];

		if(p.pos.w >= p.velocity.w) {
			p.pos = origin;
			p.pos.w = 0.0f;
			p.velocity = vec4(0.0);

			float x = rand()-0.5f;
			float y = rand()-0.5f;
			float z = rand()-0.5f;
			float w = rand()-0.5f;

			vec4 n = vec4(x, y, z, w);
			//n *= rand();

			p.pos.x += n.x;
			p.pos.y += n.y;
			p.pos.z += n.z;

			p.velocity.w = w;

		}
	
		//p.pos.w += dt;
		p.velocity += normalize(origin-p.pos)*length(fract(p.pos.xyzw)*300.f)*dt*dt;

		p.pos += p.velocity*dt;


		particle_array[id] = p;
	}
	

}
