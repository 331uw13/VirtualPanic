#pragma once
#include <glm/glm.hpp>

#include "shader.hpp"
#include "mouse.hpp"

namespace vpanic {

	enum class MoveDir {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FORWARD,
		BACK
	};

	class Camera {
	public:
		glm::vec3 pos    { glm::vec3(0.0f) };
		glm::vec3 front  { glm::vec3(0.0f, 0.0f, -1.0f) };
		
		float yaw    { 0.0f };
		float pitch  { 0.0f };
		float roll   { 0.0f };
		float fov    { 70.f };
		float z_near  { 0.1f };
		float z_far   { 1000.f };
		float aspect_ratio  { 1.0f };
		float sensetivity   { 0.124f };
		float delta_time    { 0.0f };
		bool freecam  { true };

		glm::mat4 view        { glm::mat4(1.0f) };
		glm::mat4 projection  { glm::mat4(1.0f) };

		void update(); 

		void move(MoveDir t_direction, const float t_speed);
		void look_at_point(const glm::vec3& t_point);
		void look_at_mouse(const MouseData& t_data);
		glm::vec3 point_from_front(const float t_distance);

		// TODO: is point in radius from front ?

	private:
		glm::vec3 m_up { glm::vec3(0.0f, 1.0f, 0.0f) };
		glm::vec3 _rot_xz(const float t_x, const float t_z);
	};

}
