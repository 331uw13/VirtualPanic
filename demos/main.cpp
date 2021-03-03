/*
	
	libVirtualPanic - Simple Demo

*/
#include <VirtualPanic/virtual_panic.hpp>

// you probably dont want to have these as global
static vpanic::Engine engine;
static vpanic::Shape shape;
static vpanic::Shader shader;
static vpanic::Camera camera;

static bool light_follow_camera { false };
static constexpr float camera_speed  { 0.2f };


void update() {

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

	// update shader uniforms
	shader.use();
	if(light_follow_camera) {
		shader.set_vec3("light_pos", camera.pos);
	}
	shader.set_color("light_color", vpanic::Color(255, 228, 190));

	// finally draw it
	shape.draw(shader);
}


void setup() {
	// now compile the shader code
	// no need to worry about vertex shader because its already there
	shader.load("shader.glsl", 330); // '330' here is for GLSL version
	
	// tell engine that "here is the camera you can update it"
	engine.use_camera(&camera);

	// now get vertex data for shape and calculate normals
	// you can do this all by yourself if you want
	std::vector<vpanic::Vertex> data;
	vpanic::add_box_data(data);
	vpanic::set_normals(data);
	//vpanic::set_normals(data, vpanic::SMOOTH_NORMALS);
	shape.load(data);

	shape.color = vpanic::Color(240, 240, 240);
	shape.pos = glm::vec3(0.0f, -1.0f, 0.0f);
	shape.scale = glm::vec3(10.f, 2.0f, 10.f);
	camera.pos = glm::vec3(0.0f, 2.0f, 0.0f);

	// set default light position
	shader.use();
	shader.set_vec3("light_pos", glm::vec3(0.0f, 2.0f, 0.0f));

	// lock mouse to center of the screen
	engine.lock_mouse(true);

	// finally let engine know about these so it can update model, view and projection matrices
	engine.setup_shaders({ &shader }); 
	
	engine.background_color = vpanic::Color(20, 20, 20);
}


void keydown(uint8_t t_key) {
	//printf("keydown: 0x%x / %c\n", t_key);
	if(t_key == 'e') {
		light_follow_camera =! light_follow_camera;
	}
}


void mouse_moved(const vpanic::MouseData& t_mdata) {
	// tell camera where mouse coordinates are going
	// this will update 'camera.yaw', camera.pitch'
	// then in engine side it will just call 'camera.update()'
	// which updates 'camera.view' and 'camera.projection'
	// why its in engine side?: it needs to set camera matrix for uniform buffer
	camera.look_at_mouse(t_mdata);

	// you can also do this:
	/*
	camera.yaw += t_mdata.delta_x*camera.sensetivity;
	camera.pitch += -t_mdata.delta_y*camera.sensetivity;
	vpanic::clamp<float>(camera.pitch, -89.9f, 89.9f);
	*/
}


int main() {

	// initialize and check if its ok to continue
	engine.init("Simple Demo", glm::vec2(1200, 900));
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



