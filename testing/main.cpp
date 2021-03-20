/*
	
	libVirtualPanic - Simple Demo

*/
#include <VirtualPanic/virtual_panic.hpp>

static vpanic::Engine engine;
static vpanic::Shape shape;
static vpanic::Shape ground;
static vpanic::Shape light_plane;
static vpanic::Shader shader;
static vpanic::Shader particle_shader;
static vpanic::Camera camera;
static vpanic::ShapeArray pilars;

static vpanic::Vec3 light_pos;
static vpanic::ParticleSystem particle_system;
static vpanic::Vec3 particle_system_origin { vpanic::Vec3(10, 3, 0) };
static vpanic::Vec3 prev_particle_system_origin { vpanic::Vec3(0, 3, 0) };
static float particle_system_dist { 0 };

static bool menu_open { true };
static bool light_follow_camera { false };
static float camera_speed  { 3.75f };
static float default_camera_speed  { 3.75f };

struct Player {
	
	const float height { 2.7f };
	const float fly_speed { 5.5f };
	const float walk_speed { 3.75f };

} player;

struct LightSettings {

	vpanic::Color color { vpanic::Color(255, 228, 190) };
	float radius { 145.f };
	float brightness { 1.0f };

} light_settings;

struct ParticleSettings {
	float scale { 0.02f };
	float scale_add { 0.0f };
	float mass { 1.0f };
	int lifetime_min { 500 };
	int lifetime_max { 2500 };
	int count { 500 };

	float velocity = 1.5f;
	//vpanic::Vec3 vel_min;
	//vpanic::Vec3 vel_max;
	vpanic::Vec3 acc;

	vpanic::Color start_color { vpanic::Color(230, 15, 15, 255) };
	vpanic::Color end_color { vpanic::Color(255, 60, 0, 25) };

} particle_settings;

struct PilarSettings {

	float height { 30.0f };
	float width { 0.8f };
	float range { 50.f };
	int count { 30 };

} pilar_settings;

void create_pilars() {
	std::vector<vpanic::Vertex> data;
	vpanic::add_box_data(data);
	vpanic::set_normals(data);

	pilars.unload();
	pilars.reserve(pilar_settings.count);
	pilars.load(data);

	for(int i = 0; i < pilar_settings.count; i++) {
	
		float x = vpanic::random(-pilar_settings.range, pilar_settings.range);
		float z = vpanic::random(-pilar_settings.range, pilar_settings.range);

		vpanic::Matrix matrix(1.0f);
		matrix.translate(vpanic::Vec3(x, pilar_settings.height/2, z));
		matrix.scale(vpanic::Vec3(pilar_settings.width, pilar_settings.height, pilar_settings.width));

		pilars.set_matrix(i, matrix);
		pilars.set_color(i, vpanic::Color(50));
	}
}

void command_callback(const std::vector<std::string>& args) {
	if(args.empty()) { return; }

	if(args[0] == "exit") {
		engine.request_shutdown();
	}

	vpanic::Console::instance().print("No handler for \"%s\"", args[0].c_str());

}

void update() {

	static bool only_particle_system = false;

	if(!menu_open) {
		
		if(vpanic::keyboard::keydown(vpanic::keyboard::Mod::CTRL)) {
			camera_speed = default_camera_speed*4.5f;
		}
		else {
			camera_speed = default_camera_speed;
		}
		
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
		if(camera.freecam) {
			if(vpanic::keyboard::keydown(vpanic::keyboard::Mod::SPACE)) {
				camera.move(vpanic::MoveDir::UP, camera_speed);
			}
			if(vpanic::keyboard::keydown(vpanic::keyboard::Mod::SHIFT)) {
				camera.move(vpanic::MoveDir::DOWN, camera_speed);
			}
		}

		if(vpanic::mouse::button_down(vpanic::mouse::LEFT)) {
			prev_particle_system_origin = particle_system.origin;
			particle_system.origin = camera.point_from_front(particle_system_dist);
		}
		else {
			particle_system_dist = vpanic::distance(camera.pos, particle_system.origin);
		}
	}



	// update shader uniforms
	if(light_follow_camera) {
		light_pos = camera.pos;
	}
	shader.use();
	shader.set_vec3("light_pos", light_pos);
	shader.set_vec3("particle_origin", particle_system.origin);
	shader.set_color("light_color", light_settings.color);
	shader.set_float("light_radius", light_settings.radius);
	shader.set_float("light_brightness", light_settings.brightness);


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

		//ImGui::ShowDemoWindow();

		vpanic::Console::instance().update();

		vpanic::Vec2 win_size = engine.window_size();
		static float menu_width = 500;

		ImGui::SetNextWindowPos(ImVec2(win_size.x-menu_width, 0));
		ImGui::SetNextWindowSize(ImVec2(menu_width, win_size.y));
		
		ImGui::Begin("Menu", (bool*)nullptr, ImGuiWindowFlags_NoCollapse);
		menu_width = ImGui::GetWindowSize().x;

		if(ImGui::Button("Quit")) {
			engine.request_shutdown();
		}
		ImGui::SameLine();
		if(ImGui::Button("Home")) {
			camera.pos = vpanic::Vec3(0.0f, player.height, 0.0f);
			camera.yaw = 0.0f;
			camera.pitch = 0.0f;
		}

		static constexpr float title_size = 1.4f;
		
		ImGui::Separator();
		ImGui::SetWindowFontScale(title_size);
		ImGui::Text("Lights");
		ImGui::SetWindowFontScale(1.0f);

		ImGui::Checkbox("Follow camera", &light_follow_camera);
		ImGui::SliderFloat("##radius", &light_settings.radius, 0.1f, 1000.f, "Radius: %1.3f");
		ImGui::SliderFloat("##brightness", &light_settings.brightness, 0.1f, 2.f, "Brightness: %2.5f");


		ImGui::Separator();
		ImGui::SetWindowFontScale(title_size);
		ImGui::Text("ParticleSystem");
		ImGui::SetWindowFontScale(1.0f);


		static int count = particle_settings.count;
		static ImVec4 start_col = vpanic::color_to_imvec4(particle_settings.start_color);
		static ImVec4 end_col = vpanic::color_to_imvec4(particle_settings.end_color);
	
		ImGui::Checkbox("Render only particle system", &only_particle_system);
		if(ImGui::Button("Recompile")) {
			particle_system.unload();
			particle_system.init();
		}
		/*
		ImGui::Text("Count: %i", particle_system.get_real_count());
		ImGui::SameLine();
		ImGui::Text("Blobs: %i", particle_system.get_blob_count());
		*/
		if(ImGui::SliderInt("##count", &count, 1, 100000, "Count: %i")) {
			particle_system.resize(count);
		}

		ImGui::DragFloat("##scale", &particle_settings.scale, 0.001f, 0.005f, 5.f, "Scale %f");
		ImGui::DragFloat("##scale_add", &particle_settings.scale_add, 0.0001f, -0.001f, 0.001f, "Add To Scale %f");
		ImGui::DragFloat("##mass", &particle_settings.mass, 0.001, -0.9, 0.9, "Mass %f");
		ImGui::DragIntRange2("Lifetime", &particle_settings.lifetime_min, &particle_settings.lifetime_max, 0, 0, 10000, "Min %i", "Max %i");
		ImGui::SliderFloat("##velocity", &particle_settings.velocity, 0.0f, 8.0f, "Velocity: %f");
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

	if(!light_follow_camera) {
		
		vpanic::Matrix m(1.0f);
		m.translate(light_pos);
		m.copy_rotation(camera.view);
		m.scale(vpanic::Vec3(0.2f));
		light_plane.color = light_settings.color;
		
		light_plane.set_model_matrix(m);
		if(!only_particle_system) {
			light_plane.draw(shader);
		}

	}

	if(!only_particle_system) {
		ground.draw(shader);
		shape.draw(shader);
		pilars.draw(shader);
	}
	

	particle_system.update(shader, engine.delta_time(), camera.view);
}

bool setup() {

	vpanic::Console::instance().set_callback(command_callback);

	particle_system.init();
	particle_system.resize(particle_settings.count);

	create_pilars();

	shader.add_shader("shader.glsl", vpanic::shadertype::fragment);
	if(!shader.compile()) { return false; }

	particle_shader.add_shader("particle_shader.glsl", vpanic::shadertype::fragment);
	if(!particle_shader.compile()) { return false; }

	// tell engine that "here is the camera you can update it"
	engine.use_camera(&camera);

	// now get vertex data for shape and calculate normals
	// you can do this all by yourself if you want
	std::vector<vpanic::Vertex> data;
	vpanic::add_box_data(data);
	vpanic::set_normals(data);
	shape.load(data);

	data.clear();
	vpanic::add_plane_data(data, vpanic::USING_TRIANGLE_STRIP);
	light_plane.load(data);
	vpanic::set_normals(data);
	ground.load(data);

	ground.pos = vpanic::Vec3(0.0f);
	ground.scale = vpanic::Vec3(300.f);
	ground.rotation.x = -90.f;
	ground.color = vpanic::Color(30, 35, 30);

	shape.color = vpanic::Color(230, 45, 24);
	shape.pos = vpanic::Vec3(3.0f, 0.5f, 0.0f);
	camera.pos = vpanic::Vec3(0.0f, 2.0f, 0.0f);



	// set default light position
	light_pos = vpanic::Vec3(0.0f, 4.0f, 0.0f);
	shader.use();
	shader.set_vec3("light_pos", light_pos);
	light_plane.pos = light_pos;

	// lock mouse to center of the screen
	//engine.lock_mouse(true);

	// finally let engine know about these so it can update model, view and projection matrices
	engine.setup_shaders({ &shader, &particle_shader }); 
	//engine.background_color = vpanic::Color(240, 240, 255);
	//engine.background_color = vpanic::Color(20, 20, 20);

	engine.fullscreen(true);

	// some imgui setup
	ImGui::GetIO().Fonts->AddFontFromFileTTF("font.otf", 16.f);

	ImGuiStyle& s = ImGui::GetStyle();

	s.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.9f);
	s.Colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.15f, 0.17f, 0.8f);
	s.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.25f, 0.27f, 0.8f);

	camera.freecam = false;
	camera.pos.y = player.height;
	vpanic::Console::instance().focus();

	return true;
}


void keydown(uint8_t t_key) {
	
	switch(t_key) {
	
		case 0x09:
		case 0xa7:
			menu_open =! menu_open;
			engine.lock_mouse(!menu_open);
			vpanic::Console::instance().focus();
			break;

		case 'v':
			if(!menu_open) {
				default_camera_speed = ((camera.freecam =! camera.freecam)) ? player.fly_speed : player.walk_speed;
			}
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
	if(!setup()) {
		engine.quit();
		return -1;
	}

	// 'engine.quit()' is called on engine destructor or engine state 'quit' is set
	// so no need to call that after this
	engine.start();
}



