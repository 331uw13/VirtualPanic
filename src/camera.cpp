#include "camera.hpp"
#include "utils.hpp"
#include "math.hpp"


namespace vpanic {

	void Camera::update() {
		front = Vec3(cos(to_radians(yaw))*cos(to_radians(pitch)), sin(to_radians(pitch)), sin(to_radians(yaw))*cos(to_radians(pitch)));
		front.normalize_self();

		// projection matrix for camera
		get_projection(projection, fov, aspect_ratio, z_near, z_far);

		// view matrix for camera
		Vec3 center = pos+front;
		Vec3 f(center-pos);
		f.normalize_self();

		Vec3 s(cross(f, Vec3(0.0f, 1.0f, 0.0f)));
		s.normalize_self();

		const Vec3 u(cross(s, f));

		view[0].x = s.x;
		view[1].x = s.y;
		view[2].x = s.z;

		view[0].y = u.x;
		view[1].y = u.y;
		view[2].y = u.z;

		view[0].z = -f.x;
		view[1].z = -f.y;
		view[2].z = -f.z;

		view[3].x = -dot(s, pos);
		view[3].y = -dot(u, pos);
		view[3].z =  dot(f, pos);
	}

	Vec3 Camera::_rot_xz(const float t_x, const float t_z) {
		return Vec3(cos(to_radians(yaw))*cos(to_radians(t_x)), 0, sin(to_radians(yaw))*cos(to_radians(t_z)));
	}

	void Camera::move(MoveDir t_direction, const float t_speed) {
		const float s = t_speed*delta_time;
		switch(t_direction) {	
			case MoveDir::UP:
				pos.y += s;
				break;
			
			case MoveDir::DOWN:
				pos.y -= s;
				break;
			
			case MoveDir::LEFT:
				if(freecam) {
					pos -= cross(front, m_up)*s;
				} else {
					pos -= cross(_rot_xz(front.x, front.z), m_up)*s;
				}
				break;
			
			case MoveDir::RIGHT:
				if(freecam) {
					pos += cross(front, m_up)*s;
				} else {
					pos += cross(_rot_xz(front.x, front.z), m_up)*s;
				}
				break;
			
			case MoveDir::FORWARD:
				if(freecam) {
					pos += front*s;
				} else {
					pos.x += cos(to_radians(yaw))*cos(to_radians(front.x))*s;
					pos.z += sin(to_radians(yaw))*cos(to_radians(front.z))*s;
				}
				break;
			
			case MoveDir::BACK:
				if(freecam) {
					pos -= front*s;
				} else {
					pos.x -= cos(to_radians(yaw))*cos(to_radians(front.x))*s;
					pos.z -= sin(to_radians(yaw))*cos(to_radians(front.z))*s;
				}
				break;

			default: break;
		}
	}
		
	void Camera::look_at_point(const Vec3& t_point) {
	}
	
	void Camera::look_at_mouse(const MouseData& t_data) {
		const float s = sensetivity;
		yaw += (t_data.delta_x)*s;
		pitch += (-t_data.delta_y)*s;
		clamp<float>(pitch, -89.9f, 89.9f);
	}
	
	Vec3 Camera::point_from_front(const float t_distance) {
		return Vec3(pos.x+cos(to_radians(yaw))*(t_distance*cos(to_radians(pitch))),
				pos.y+t_distance*sin(to_radians(pitch)),
				pos.z+sin(to_radians(yaw))*(t_distance*cos(to_radians(pitch))));
	}



}
