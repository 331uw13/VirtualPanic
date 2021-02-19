#include <cmath>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "libs/imgui/imgui.h"

#include "utils.hpp"
#include "theme.hpp"
#include "settings.hpp"
#include "console.hpp"

static int g_seed { 0 };

namespace vpanic {

	float fade(const float t) {
		return t * t * t * (t * (t + 6 - 15) + 10);
	}

	float lerp(const float t, const float start, const float end) {
		return start + (end - start) * t;
	}

	float norm(const float value, const float min, const float max) {
		return (value - min) / (max - min);
	}
	
	float map(const float value, const float src_min, const float src_max,
			const float dst_min, const float dst_max) {
		return lerp(dst_min, dst_max, norm(value, src_min, src_max));
	}

	float angle_between(const glm::vec2& p0, const glm::vec2& p1) {
		return atan2(p1.x-p0.x, p1.y-p0.y);
	}

	float distance(const glm::vec3& p0, const glm::vec3& p1) {
		const float dx = p1.x - p0.x;
		const float dy = p1.y - p0.y;
		const float dz = p1.z - p0.z;
	
		return sqrt((dx*dx)+(dy*dy)+(dz*dz));
	}

	float random(const float min, const float max) {
		return min + static_cast<float>(fast_rand()) / (static_cast<float>((float)0x7FFF / (float)(max - min)));
	}
	
	glm::vec3 random_point(const glm::vec3& min, const glm::vec3& max) {
		return glm::vec3(random(min.x, max.x), random(min.y, max.y), random(min.z, max.z));
	}

	int fast_rand() {
		g_seed = (0x343FD * g_seed + 0x269EC3);
		return (g_seed >> 16) & 0x7FFF;
	}

	void set_seed(const int new_seed) {
		g_seed = new_seed;
	}

	Color mix_color(const Color& start, const Color& end, float t) {
		return Color(lerp(start.r, end.r, t), lerp(start.g, end.g, t), 
				lerp(start.b, end.b, t), lerp(start.a, end.a, t));
	}

	void invert_color(Color& color) {
		color.r = 255-color.r;
	   	color.g = 255-color.g;
		color.b = 255-color.b;
	}
	
	Color illuminate(const Color& color, const int8_t t) {
		
		// NOTE: this is bad because if for example 'color.r+t' goes negative value
		// it will go back to 255 because im using unsigned
		uint8_t r = color.r+t;
		uint8_t g = color.g+t;
		uint8_t b = color.b+t;

		clamp<uint8_t>(r, 0, 255);
		clamp<uint8_t>(g, 0, 255);
		clamp<uint8_t>(b, 0, 255);

		return Color(r, g, b);
	}

	ImVec4 color_to_imvec4(const Color& color) {
		return ImVec4(
				static_cast<float>(color.r)/255.0f,
				static_cast<float>(color.g)/255.0f,
				static_cast<float>(color.b)/255.0f,
				static_cast<float>(color.a)/255.0f);
	}

	void read_imgui_theme(const char* filename) {

		Theme theme(filename);
		ImGuiStyle& style = ImGui::GetStyle();

		// TODO: add more things, thats not all
		Color background   = theme.get("background");
		Color border       = theme.get("border");
		Color title        = theme.get("title");
		Color text         = theme.get("text");
		Color text_selected = theme.get("text_selected");
		Color frame        = theme.get("frame");
		Color button       = theme.get("button");
		Color tab          = theme.get("tab");
		Color tab_unfocus  = theme.get("tab_unfocus");
		Color header       = theme.get("header");
		Color checkmark    = theme.get("checkmark");
		Color slider_grab  = theme.get("slider_grab");
		Color resize_grip  = theme.get("resize_grip");
		Color separator    = theme.get("separator");

		// TODO: settings for these 2 values
		const uint8_t hovered  = 10;
		const uint8_t active   = 20;

		style.Colors[ImGuiCol_Text]       = color_to_imvec4(text);
		style.Colors[ImGuiCol_TextSelectedBg] = color_to_imvec4(text_selected);
		style.Colors[ImGuiCol_Border]     = color_to_imvec4(border);
		style.Colors[ImGuiCol_WindowBg]   = color_to_imvec4(background);
		style.Colors[ImGuiCol_CheckMark]  = color_to_imvec4(checkmark);
		
		style.Colors[ImGuiCol_FrameBg]         = color_to_imvec4(frame);
		style.Colors[ImGuiCol_FrameBgHovered]  = color_to_imvec4(illuminate(frame, hovered));
		style.Colors[ImGuiCol_FrameBgActive]   = color_to_imvec4(illuminate(frame, active));
		
		style.Colors[ImGuiCol_TitleBg]          = color_to_imvec4(title);
		style.Colors[ImGuiCol_TitleBgActive]    = color_to_imvec4(illuminate(title, active));
		style.Colors[ImGuiCol_TitleBgCollapsed] = color_to_imvec4(illuminate(title, -(active/2)));
		
		style.Colors[ImGuiCol_Separator]           = color_to_imvec4(separator);
		style.Colors[ImGuiCol_SeparatorHovered]  = color_to_imvec4(illuminate(separator, hovered));
		style.Colors[ImGuiCol_SeparatorActive]   = color_to_imvec4(illuminate(separator, active));
		
		style.Colors[ImGuiCol_Button]         = color_to_imvec4(button);
		style.Colors[ImGuiCol_ButtonHovered]  = color_to_imvec4(illuminate(button, hovered));
		style.Colors[ImGuiCol_ButtonActive]   = color_to_imvec4(illuminate(button, active));
		
		style.Colors[ImGuiCol_Tab]            = color_to_imvec4(tab);
		style.Colors[ImGuiCol_TabHovered]     = color_to_imvec4(illuminate(tab, hovered));
		style.Colors[ImGuiCol_TabActive]      = color_to_imvec4(illuminate(tab, active));
		style.Colors[ImGuiCol_TabUnfocused]   = color_to_imvec4(tab_unfocus);
		style.Colors[ImGuiCol_TabUnfocusedActive]  = color_to_imvec4(illuminate(tab_unfocus, active));

		style.Colors[ImGuiCol_Header]         = color_to_imvec4(header);
		style.Colors[ImGuiCol_HeaderHovered]  = color_to_imvec4(illuminate(header, hovered));
		style.Colors[ImGuiCol_HeaderActive]   = color_to_imvec4(illuminate(header, active));

		style.Colors[ImGuiCol_SliderGrab]         = color_to_imvec4(slider_grab);
		style.Colors[ImGuiCol_SliderGrabActive]   = color_to_imvec4(illuminate(slider_grab, active));
		
		style.Colors[ImGuiCol_ResizeGrip]         = color_to_imvec4(resize_grip);
		style.Colors[ImGuiCol_ResizeGripHovered]  = color_to_imvec4(illuminate(resize_grip, hovered));
		style.Colors[ImGuiCol_ResizeGripActive]   = color_to_imvec4(illuminate(resize_grip, active));

	}
	
	void add_triangle_data(std::vector<Vertex>& out) {
	}

	void add_plane_data(std::vector<Vertex>& out, const int t_settings) {
		
		std::vector<Vertex> tmp = [t_settings]() {
			
			if(t_settings == DOUBLE_SIDE) {
				return std::vector<Vertex> {
					Vertex(-0.5f, -0.5f,  0.0f),
					Vertex( 0.5f,  0.5f,  0.0f),
					Vertex( 0.5f, -0.5f,  0.0f),
					Vertex( 0.5f,  0.5f,  0.0f),
					Vertex(-0.5f, -0.5f,  0.0f),
					Vertex(-0.5f,  0.5f,  0.0f),
	
					Vertex(-0.5f, -0.5f, 0.0f),
					Vertex( 0.5f, -0.5f, 0.0f),
					Vertex( 0.5f,  0.5f, 0.0f),
					Vertex( 0.5f,  0.5f, 0.0f),
					Vertex(-0.5f,  0.5f, 0.0f),
					Vertex(-0.5f, -0.5f, 0.0f),
				};
			}
			else {
				return std::vector<Vertex> {
					Vertex(-0.5f, -0.5f,  0.0f),
					Vertex( 0.5f,  0.5f,  0.0f),
					Vertex( 0.5f, -0.5f,  0.0f),
					Vertex( 0.5f,  0.5f,  0.0f),
					Vertex(-0.5f, -0.5f,  0.0f),
					Vertex(-0.5f,  0.5f,  0.0f),
				};
			}

		}();

		out.resize(out.size()+tmp.size());
		out.assign(tmp.begin(), tmp.end());

	}

	void add_sphere_data(std::vector<Vertex>& out) {
	}
	
	void add_box_data(std::vector<Vertex>& out) {	
		std::vector<Vertex> tmp = {
			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f, -0.5f),
			Vertex( 0.5f,  0.5f, -0.5f),
			Vertex( 0.5f,  0.5f, -0.5f),
			Vertex(-0.5f,  0.5f, -0.5f),
			Vertex(-0.5f, -0.5f, -0.5f),

			Vertex(-0.5f, -0.5f,  0.5f),
			Vertex( 0.5f,  0.5f,  0.5f),
			Vertex( 0.5f, -0.5f,  0.5f),
			Vertex( 0.5f,  0.5f,  0.5f),
			Vertex(-0.5f, -0.5f,  0.5f),
			Vertex(-0.5f,  0.5f,  0.5f),

			Vertex(-0.5f,  0.5f,  0.5f),
			Vertex(-0.5f, -0.5f, -0.5f), 
			Vertex(-0.5f,  0.5f, -0.5f), 
			Vertex(-0.5f, -0.5f, -0.5f), 
			Vertex(-0.5f,  0.5f,  0.5f), 
			Vertex(-0.5f, -0.5f,  0.5f), 

			Vertex( 0.5f,  0.5f,  0.5f),
			Vertex( 0.5f,  0.5f, -0.5f),
			Vertex( 0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f,  0.5f),
			Vertex( 0.5f,  0.5f,  0.5f),

			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f,  0.5f),
			Vertex( 0.5f, -0.5f, -0.5f),
			Vertex( 0.5f, -0.5f,  0.5f),
			Vertex(-0.5f, -0.5f, -0.5f),
			Vertex(-0.5f, -0.5f,  0.5f),
		
			Vertex(-0.5f,  0.5f, -0.5f),
			Vertex( 0.5f,  0.5f, -0.5f),
			Vertex( 0.5f,  0.5f,  0.5f),
			
			Vertex( 0.5f,  0.5f,  0.5f),
			Vertex(-0.5f,  0.5f,  0.5f),
			Vertex(-0.5f,  0.5f, -0.5f)
		};

		out.resize(out.size()+tmp.size());
		out.assign(tmp.begin(), tmp.end());
	}

	void set_normals(std::vector<Vertex>& out, const int t_settings) {
		if(out.empty() || out.size() < 3) { return; }

		glm::vec3 triangle[3];

		for(uint32_t i = 0; i < out.size(); i+=3) {
			triangle[0] = out[i].point;
			triangle[1] = out[i+1].point;
			triangle[2] = out[i+2].point;

			glm::vec3 normal = glm::cross(triangle[1]-triangle[0], triangle[2]-triangle[0]);	
			normal = -(normal/glm::length(normal));

			if(t_settings == SMOOTH_NORMALS) {
				normal = -normal;
			}

			out[i].normal = normal;
			out[i+1].normal = normal;
			out[i+2].normal = normal;
		}
	}

	void invert_normals(std::vector<Vertex>& out) {
	}


}






