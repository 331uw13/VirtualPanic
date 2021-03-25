/*
	
	libVirtualPanic - Testing

	this is big mess because its just for testing...

*/
#include <VirtualPanic/virtual_panic.hpp>

static vpanic::Engine engine;
static vpanic::Shape shape;
static vpanic::Shape ground;
static vpanic::Shape light_plane;
static vpanic::Shader shader;
static vpanic::Shader particle_shader;
static vpanic::Shader particle_frag;
static vpanic::Camera camera;
static vpanic::ShapeArray pilars;

static vpanic::Vec3 light_pos;
vpanic::ParticleSystem particle_system;
static float particle_system_dist { 0 };

static bool menu_open { true };
static bool light_follow_camera { true };
static float camera_speed  { 3.75f };
static float default_camera_speed  { 3.75f };
static std::string shader_source { "" };

struct Player {
	const float height { 2.7f };
	const float fly_speed { 5.5f };
	const float walk_speed { 3.75f };
} player;

struct LightSettings {
	vpanic::Color color { vpanic::Color(255, 228, 190) };
	float radius { 145.f };
	float brightness { 0.5f };
} light_settings;

struct ParticleSettings {
	int count { 5000 };	
	bool render { false };
	std::string src { "" };
} particle_settings;

struct PilarSettings {
	float height { 30.0f };
	float width { 0.5f };
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

void imgui_title(const char* t_title) {
	ImGui::Separator();
	ImGui::SetWindowFontScale(1.4f);
	ImGui::Text(t_title);
	ImGui::SetWindowFontScale(1.0f);
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

		ImGui::Begin("Editor");
		{
			if(ImGui::Button("Compile")) {
				shader.unload();
				shader.add_src_from_memory(shader_source, vpanic::FRAGMENT_SHADER);
				shader.compile();
			}
			ImGui::SameLine();
			if(ImGui::Button("Dump to stdout")) {
				printf("\033[33m############################################################\033[0m\n");
				printf("%s\n", shader_source.c_str());
			}

			const ImVec2& winsize = ImGui::GetWindowSize();
			vpanic::ImGuiExt::TextEdit("##shader_source", &shader_source, ImVec2(winsize.x-20, winsize.y-70));
		}
		ImGui::End();


		//ImGui::ShowDemoWindow();

		vpanic::Console::instance().update();

		vpanic::Vec2 win_size = engine.window_size();
		static float menu_width = 600;

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


		imgui_title("Lights");
		{
			static ImVec4 color = vpanic::color_to_imvec4(light_settings.color);
			ImGui::Checkbox("Follow camera", &light_follow_camera);
			ImGui::SliderFloat("##radius", &light_settings.radius, 0.1f, 1000.f, "Radius: %1.3f");
			ImGui::SliderFloat("##brightness", &light_settings.brightness, 0.1f, 2.f, "Brightness: %2.5f");
			if(vpanic::ImGuiExt::ColorEdit("Color", color)) {
				light_settings.color = vpanic::imvec4_to_color(color);
			}
		}

		imgui_title("ParticleSystem");
		{

			if(ImGui::SliderInt("##count", &particle_settings.count, 1, 500000)) {
				particle_system.resize(particle_settings.count);
			}
			ImGui::Checkbox("Render", &particle_settings.render);
			if(ImGui::Button("Recompile")) {
				particle_shader.unload();
				particle_shader.add_src_from_memory(particle_settings.src, vpanic::COMPUTE_SHADER);
				particle_shader.compile();
			}

			vpanic::ImGuiExt::TextEdit("##test", &particle_settings.src, ImVec2(ImGui::GetWindowSize().x-20, 400));

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
		light_plane.draw(shader);

	}

	ground.draw(shader);
	shape.draw(shader);
	pilars.draw(shader);
	
	if(particle_settings.render) {
		particle_system.update(particle_frag, particle_shader, engine.delta_time());
	}
}

bool setup() {

	vpanic::Console::instance().set_callback(command_callback);

	vpanic::read_file("shader_source.txt", &shader_source);
	vpanic::read_file("particle_shader.glsl", &particle_settings.src);
	particle_system.init();
	create_pilars();

	shader.add_src("shader.glsl", vpanic::FRAGMENT_SHADER);
	if(!shader.compile()) { return false; }

	particle_shader.add_src("particle_shader.glsl", vpanic::COMPUTE_SHADER);
	if(!particle_shader.compile()) { return false; }

	particle_frag.add_src("particle_fragment.glsl", vpanic::FRAGMENT_SHADER);
	if(!particle_frag.compile()) { return false; }
	
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
	engine.setup_shaders({ &shader }); 
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

	particle_system.init();

	return true;
}


void keydown(uint8_t t_key) {
	
	switch(t_key) {
	
		//case 0x09:
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
	engine.init("libVirtualPanic [Testing]", vpanic::Vec2(1200, 900));
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



