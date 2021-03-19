/*
	
	libVirtualPanic - Simple Demo

*/
#include <VirtualPanic/virtual_panic.hpp>

static vpanic::Engine engine;
static vpanic::Shape shape;
static vpanic::Shader shader;
static vpanic::Shader particle_shader;
static vpanic::Camera camera;

static vpanic::ParticleSystem particle_system;
static vpanic::Vec3 particle_system_origin { vpanic::Vec3(10, 3, 0) };
static vpanic::Vec3 prev_particle_system_origin { vpanic::Vec3(0, 3, 0) };
static float particle_system_dist { 0 };

static bool menu_open { false };
static bool light_follow_camera { false };
static constexpr float camera_speed  { 3.7f };

struct ParticleSettings {
	float scale { 0.02f };
	float scale_add { 0.0f };
	float mass { 1.0f };
	int lifetime_min { 500 };
	int lifetime_max { 2500 };
	int count { 20000 };

	float vel_min = -1.5f;
	float vel_max = 1.5f;
	//vpanic::Vec3 vel_min;
	//vpanic::Vec3 vel_max;
	vpanic::Vec3 acc;

	vpanic::Color start_color { vpanic::Color(230, 15, 15, 255) };
	vpanic::Color end_color { vpanic::Color(255, 60, 0, 25) };

} particle_settings;



void update() {

	if(!menu_open) {
		if(vpanic::keyboard::keydown('w')) {
			camera.move(vpanic::MoveDir::FORWARD, camera_speed);
		}
		if(vpanic::keyboard::keydown('a')) {
			camera.move(vpanic::MoveDir::LEFT, camera_speed);
		}
		if(vpanic::keyboard::keydown('s')) {
			camera.move(vpanic::MoveDir::BACK, camera_speed);
		}
		if(vpanic::keyboard::keydown('d')) {
			camera.move(vpanic::MoveDir::RIGHT, camera_speed);
		}
		if(vpanic::keyboard::keydown(vpanic::keyboard::Mod::SPACE)) {
			camera.move(vpanic::MoveDir::UP, camera_speed);
		}
		if(vpanic::keyboard::keydown(vpanic::keyboard::Mod::SHIFT)) {
			camera.move(vpanic::MoveDir::DOWN, camera_speed);
		}

		if(vpanic::mouse::button_down(vpanic::mouse::LEFT)) {
			prev_particle_system_origin = particle_system_origin;
			particle_system_origin = camera.point_from_front(particle_system_dist);
			particle_settings.acc = (particle_system_origin - prev_particle_system_origin)/10.f;
		}
		else {
			particle_system_dist = vpanic::distance(camera.pos, particle_system_origin);
			particle_settings.acc = vpanic::Vec3(0.0f);
		}
	}



	// update shader uniforms
	shader.use();
	if(light_follow_camera) {
		shader.set_vec3("light_pos", camera.pos);
	}
	shader.set_color("light_color", vpanic::Color(255, 228, 190));

	particle_shader.use();
	if(light_follow_camera) {
		particle_shader.set_vec3("light_pos", camera.pos);
	}
	particle_shader.set_color("light_color", vpanic::Color(255, 228, 190));


	// finally draw it


	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(400, 100));
	ImGui::Begin("##info stuff", (bool*)nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowFontScale(1.3f);
	ImGui::VPanic::TextRGB("<AAAAAA>LOOKING AT:  <eb4d4b>%1.2f, <2ecc71>%1.2f, <3498db>%1.2f", camera.front.x, camera.front.y, camera.front.z);
	ImGui::VPanic::TextRGB("<AAAAAA>POSITION:    <eb4d4b>%1.2f, <2ecc71>%1.2f, <3498db>%1.2f", camera.pos.x, camera.pos.y, camera.pos.z);
	ImGui::VPanic::TextRGB("<AAAAAA>FPS: <e056fd>%i", (int)ImGui::GetIO().Framerate);
	ImGui::SetWindowFontScale(1.0f);
	ImGui::End();


	if(menu_open) {

		ImGui::Begin("Menu");
		if(ImGui::Button("Quit")) {
			engine.request_shutdown();
		}
		ImGui::Separator();

		static constexpr float title_size = 1.4f;

		ImGui::SetWindowFontScale(title_size);
		ImGui::Text("ParticleSystem");
		ImGui::SetWindowFontScale(1.0f);


		static int count = particle_settings.count;
		static ImVec4 start_col = vpanic::color_to_imvec4(particle_settings.start_color);
		static ImVec4 end_col = vpanic::color_to_imvec4(particle_settings.end_color);
		
		ImGui::Text("Count: %i", particle_system.get_real_count());
		ImGui::SameLine();
		ImGui::Text("Blobs: %i", particle_system.get_blob_count());

		if(ImGui::SliderInt("##count", &count, 1, 100000, "Count: %i")) {
			particle_system.resize(count);
		}

		static constexpr float velocity_range = 2.0f;
		static constexpr float mass_range = 2.f;


		ImGui::DragFloat("##scale", &particle_settings.scale, 0.001f, 0.005f, 5.f, "Scale %f");
		ImGui::DragFloat("##scale_add", &particle_settings.scale_add, 0.0001f, -0.001f, 0.001f, "Add To Scale %f");
		ImGui::DragFloat("##mass", &particle_settings.mass, 0.05, -mass_range, mass_range, "Mass %f");
		ImGui::DragIntRange2("Lifetime", &particle_settings.lifetime_min, &particle_settings.lifetime_max, 0, 0, 10000, "Min %i", "Max %i");
		ImGui::DragFloatRange2("Velocity", &particle_settings.vel_min, &particle_settings.vel_max, 0.005f, -velocity_range, velocity_range, "Min %f", "Max %f");
		//ImGui::DragFloatRange2("Velocity X", &particle_settings.vel_min.x, &particle_settings.vel_max.x, 0.05f, -velocity_range, velocity_range, "Min %f", "Max %f");
		//ImGui::DragFloatRange2("Velocity Y", &particle_settings.vel_min.y, &particle_settings.vel_max.y, 0.05f, -velocity_range, velocity_range, "Min %f", "Max %f");
		//ImGui::DragFloatRange2("Velocity Z", &particle_settings.vel_min.z, &particle_settings.vel_max.z, 0.05f, -velocity_range, velocity_range, "Min %f", "Max %f");

		if(vpanic::ImGuiExt::ColorEdit("Start color", start_col)) {
			particle_settings.start_color = vpanic::imvec4_to_color(start_col);
		}
		if(vpanic::ImGuiExt::ColorEdit("End color", end_col)) {
			particle_settings.end_color = vpanic::imvec4_to_color(end_col);
		}

		ImGui::Separator();
		ImGui::End();
	}

	shape.draw(shader);
	particle_system.update(particle_shader, engine.delta_time(), camera.view);
}

void particle_update(vpanic::Particle* p) {

	static float x = 0.0f;
	static float y = 0.0f;
	static float z = 0.0f;
	
	const float radius = 2.5f;

	if(p->dead) {
		p->dead = false;
		p->lifetime = 0.0f;
		p->pos = particle_system_origin;

		const float rand_x = vpanic::random(particle_settings.vel_min, particle_settings.vel_max);
		const float rand_y = vpanic::random(particle_settings.vel_min, particle_settings.vel_max);
		const float rand_z = vpanic::random(particle_settings.vel_min, particle_settings.vel_max);

		p->scale = vpanic::Vec3(particle_settings.scale);
		p->vel = vpanic::Vec3(rand_x, rand_y, rand_z);
		p->max_lifetime = vpanic::random(particle_settings.lifetime_min, particle_settings.lifetime_max);
		p->color = particle_settings.start_color;
		
		p->acc = particle_settings.acc*80.f;
		p->acc.y += 2.5f;
		p->acc.x *= 0.1f;
		p->acc.z *= 0.1f;

	}
	else {
		const float dt = engine.delta_time();
		const float t = 1.0f/(p->max_lifetime/p->lifetime);
		p->color = vpanic::mix_color(particle_settings.start_color, particle_settings.end_color, t);

		p->scale += particle_settings.scale_add;
		p->pos += p->vel*dt;
		
		if(!p->acc.all(0.0f) && particle_settings.mass != 0.0f) {
			p->vel += p->acc*dt/particle_settings.mass;
		}

		p->acc *= 0.99f;



	}

}

void setup() {

	particle_system.resize(particle_settings.count);
	particle_system.update_callback(particle_update);

	// now compile the shader code
	// no need to worry about vertex shader because its already there
	const int glsl_version = 330;
	shader.load("shader.glsl", glsl_version);
	particle_shader.load("particle_shader.glsl", glsl_version);
	

	// tell engine that "here is the camera you can update it"
	engine.use_camera(&camera);

	// now get vertex data for shape and calculate normals
	// you can do this all by yourself if you want
	std::vector<vpanic::Vertex> data;
	vpanic::add_box_data(data);
	vpanic::set_normals(data);
	shape.load(data);

	shape.rotation.x = -90.f;
	shape.color = vpanic::Color(240, 50, 24);
	shape.pos = vpanic::Vec3(3.0f, -1.0f, 0.0f);
	//shape.scale = vpanic::Vec3(10.f, 10.f, 10.0f);
	camera.pos = vpanic::Vec3(0.0f, 2.0f, 0.0f);

	// set default light position
	shader.use();
	shader.set_vec3("light_pos", vpanic::Vec3(0.0f, 2.0f, 0.0f));

	// lock mouse to center of the screen
	engine.lock_mouse(true);

	// finally let engine know about these so it can update model, view and projection matrices
	engine.setup_shaders({ &shader, &particle_shader }); 
	//engine.background_color = vpanic::Color(240, 240, 255);
	engine.background_color = vpanic::Color(20, 20, 20);

	engine.fullscreen(true);

	// some imgui setup
	ImGui::GetIO().Fonts->AddFontFromFileTTF("font.otf", 16.f);
}


void keydown(uint8_t t_key) {
	
	switch(t_key) {
	
		case 0x09:
		case 0xa7:
			menu_open =! menu_open;
			engine.lock_mouse(!menu_open);
			break;

			/*
		case 'q':
			engine.request_shutdown();
			break;
			*/

		default:break;
	}
	
	/*
	if(t_key == 'e') {
		light_follow_camera =! light_follow_camera;
	}
	else if(t_key == 'q') {
		engine.request_shutdown();
	}
	*/
}


void mouse_moved(const vpanic::MouseData& t_mdata) {
	// tell camera where mouse coordinates are going
	// this will update 'camera.yaw', camera.pitch'
	if(!menu_open) {
		camera.look_at_mouse(t_mdata);
	}
}


int main() {

	// initialize and check if its ok to continue
	engine.init("Simple Demo", vpanic::Vec2(1200, 900));
	if(!engine.copy_state()[vpanic::EngineState::OK]) {
		return -1;
	}

	// set callbacks
	engine.update_callback(update);
	engine.mouse_move_callback(mouse_moved);
	engine.keydown_callback(keydown);

	// do some setup before starting engine
	setup();

	// 'engine.quit()' is called on engine destructor or engine state 'quit' is set
	// so no need to call that after this
	engine.start();
}



