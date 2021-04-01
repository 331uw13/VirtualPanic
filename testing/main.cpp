/*
	
	this may be big mess but its just for testing...

*/

#include <VirtualPanic/virtual_panic.hpp>


static vpanic::Engine engine;
static vpanic::Camera camera;

static constexpr float default_camera_speed { 3.8f };
static float camera_speed { 0.0f };
static bool menu_open { false };

struct Light {
	vpanic::Vec3 pos;
	vpanic::Color color;
} light;

static vpanic::Shape ground;
static vpanic::Texture ground_texture;
static vpanic::Shader ground_shader;

static std::vector<vpanic::Shape> boxes;
static std::vector<vpanic::Vertex> box_vertices;
static vpanic::Shader box_shader;


void add_box() {
	vpanic::Shape box;
	box.load(box_vertices);
	box.pos = camera.point_from_front(2.5f);
	box.scale = vpanic::Vec3(vpanic::random(0.2f, 0.8f));
	box.color = vpanic::random_color(80, 200);
	boxes.push_back(box);
}

void command_callback(const std::vector<std::string>& args) {
	if(args.empty()) { return; }

	const size_t args_size = args.size();

	/*
	if(args[0] == "exit") {
		engine.request_shutdown();
	}
	else if(args[0] == "clear") {
		vpanic::Console::instance().clear_text();
	}
	else if(args[0] == "new") {
		if(args_size >= 2) {
			
			if(args[1] == "box") {
				add_box();
			}

		}
	}
	else if(args[0] == "gamma") {
		if(args_size >= 2) {	
			float new_gamma = vpanic::convert_str<float>(args[1].c_str());
			vpanic::clamp<float>(new_gamma, 0.5f, 5.0f);
			engine.gamma = new_gamma;

			vpanic::Console::instance().print("Engine set gamma to %f", new_gamma);

		}
	}
	else if(args[0] == "exposure") {
		if(args_size >= 2) {
			float new_exposure = vpanic::convert_str<float>(args[1].c_str());
			vpanic::clamp<float>(new_exposure, 0.1f, 5.0f);
			engine.exposure = new_exposure;

			vpanic::Console::instance().print("Engine set exposure to %f", new_exposure);
		}
	}
	else {
		vpanic::Console::instance().print("Unknown input \"%s\"", args[0].c_str());
	}
	*/

}


void imgui_title(const char* t_title) {
	ImGui::Separator();
	ImGui::SetWindowFontScale(1.4f);
	ImGui::Text(t_title);
	ImGui::SetWindowFontScale(1.0f);
}


void update() {

	if(!menu_open) {
		
		if(vpanic::keyboard::keydown(vpanic::keyboard::Mod::CTRL)) {
			camera_speed = default_camera_speed*2.0f;
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
	
	}

	// draw shapes

	ground_shader.use();
	ground_shader.set_vec3("light_pos", light.pos);
	ground_shader.set_color("light_color", light.color);
	ground.draw(ground_shader);


	box_shader.use();
	box_shader.set_vec3("light_pos", light.pos);
	box_shader.set_color("light_color", light.color);
	

	for(size_t i = 0; i < boxes.size(); i++) {
		boxes[i].draw(box_shader);
	}

	// imgui stuff

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(400, 100));
	ImGui::Begin("##info_window", (bool*)nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowFontScale(1.3f);
	vpanic::ImGuiExt::TextRGB("<AAAAAA>LOOKING AT:  <eb4d4b>%1.2f, <2ecc71>%1.2f, <3498db>%1.2f", camera.front.x, camera.front.y, camera.front.z);
	vpanic::ImGuiExt::TextRGB("<AAAAAA>POSITION:    <eb4d4b>%1.2f, <2ecc71>%1.2f, <3498db>%1.2f", camera.pos.x, camera.pos.y, camera.pos.z);
	vpanic::ImGuiExt::TextRGB("<AAAAAA>FPS: <e056fd>%i", (int)ImGui::GetIO().Framerate);
	ImGui::SetWindowFontScale(1.0f);
	ImGui::End();

	
	if(menu_open) {

		vpanic::console::render();

		//vpanic::Console::instance().update();

		/*
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
			camera.pos = vpanic::Vec3(0.0f, 0.0f, 0.0f);
			camera.yaw = 0.0f;
			camera.pitch = 0.0f;
		}

		ImGui::Separator();
		ImGui::End();
		*/
	}


}

bool setup() {
	//vpanic::Console::instance().callback(command_callback);
	ImGui::GetIO().Fonts->AddFontFromFileTTF("font.otf", 16.f);

	std::vector<vpanic::Vertex> plane_vertices;

	vpanic::set_box_data(box_vertices);
	vpanic::set_normals(box_vertices);

	vpanic::set_plane_data(plane_vertices);
	vpanic::set_normals(plane_vertices);
	vpanic::set_texcoords(plane_vertices, 15);

	box_shader.add_src("box_shader.glsl", vpanic::FRAGMENT_SHADER);
	box_shader.compile();

	ground_shader.add_src("ground_shader.glsl", vpanic::FRAGMENT_SHADER);
	ground_shader.compile();

	ground_texture.load("ground.jpg");

	ground.load(plane_vertices);
	ground.pos = vpanic::Vec3(0.0f, 0.0f, 0.0f);
	ground.scale = vpanic::Vec3(50.f);
	ground.rotation.x = -90.f;
	ground.color = vpanic::Color(30);
	ground.attach_texture(&ground_texture);


	light.pos = vpanic::Vec3(0.0f, 4.f, 0.0f);
	light.color = vpanic::Color(255, 180, 120);
	camera.pos.y = 3.0f;
	engine.use_camera(&camera);
	engine.fullscreen(true);
	engine.lock_mouse(true);
	//engine.background_color = vpanic::Color(220, 220, 230);
	return true;
}


void keydown(uint8_t t_key) {
	
	switch(t_key) {
	
		//case 0x09:
		case 0xa7:
			menu_open =! menu_open;
			engine.lock_mouse(!menu_open);
			vpanic::console::state().set(vpanic::ConsoleState::FOCUS_INPUT_BOX);
			break;

		case 'f':
			if(!menu_open) {
				add_box();
			}
			break;
			
		case 'r':
			vpanic::console::print("random float: <00FF00>%f", vpanic::random(0.0f, 1000.f));
			break;
			
			// ...

		default: break;
	}
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
		engine.quit(); // FIXME
		return -1;
	}

	// 'engine.quit()' is called on engine destructor or engine state 'quit' is set
	// so no need to call that after this
	engine.start();
}



