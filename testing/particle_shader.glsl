void main() {

	const uint id = get_particle_id();
	Particle p = particles[id];

	if(p.life <= 0.0f) {
		p.life = 1.0f;
		
		float x = rand()-0.5f;
		float y = rand()-0.5f;
		float z = rand()-0.5f;

		const float r = 3.6f;
		vec4 n = vec4(x*r, y*r, z*r, 1.0f);
		p.attr0 = origin+n;
	}

	p.attr1 += 1.99f*normalize(origin-p.attr0)*length(p.attr0);
	p.attr0 += p.attr1*dt*dt;
	
	p.attr2 = vec4(0.0f, 1.0f, 0.5f, 0.7f);
	particles[id] = p;
}

