#include <cmath>
#include <vector>
#include "libs/imgui/imgui.h"

#include "math.hpp"
#include "utils.hpp"
#include "theme.hpp"
#include "settings.hpp"
#include "console.hpp"

static int g_seed { 0 };


namespace vpanic {

	float fade(const float t) {
		return t * t * t * (t * (t + 6 - 15) + 10);
	}

	float lerp(const float start, const float end, const float t) {
		return start + (end - start) * t;
	}

	float norm(const float value, const float min, const float max) {
		return (value - min) / (max - min);
	}
	
	float map(const float value, const float src_min, const float src_max,
			const float dst_min, const float dst_max) {
		return lerp(dst_min, dst_max, norm(value, src_min, src_max));
	}

	float distance(const Vec3& p0, const Vec3& p1) {
		const float dx = p1.x - p0.x;
		const float dy = p1.y - p0.y;
		const float dz = p1.z - p0.z;
	
		return sqrt((dx*dx)+(dy*dy)+(dz*dz));
	}

	float random(const float min, const float max) {
		return min + static_cast<float>(fast_rand()) / (static_cast<float>((float)0x7FFF / (float)(max - min)));
	}
	
	int fast_rand() {
		g_seed = (0x343FD * g_seed + 0x269EC3);
		return (g_seed >> 16) & 0x7FFF;
	}

	void set_seed(const int new_seed) {
		g_seed = new_seed;
	}

	Color mix_color(const Color& start, const Color& end, float t) {
		return Color(
				lerp((float)start.r, (float)end.r, t), 
				lerp((float)start.g, (float)end.g, t),
			   	lerp((float)start.b, (float)end.b, t),
			   	lerp(start.a, end.a, t)
				);
	}

	void invert_color(Color& color) {
		color.r = 255-color.r;
	   	color.g = 255-color.g;
		color.b = 255-color.b;
	}
	
	Color illuminate(const Color& color, const int8_t t) {
		
		int8_t r = color.r+t;
		int8_t g = color.g+t;
		int8_t b = color.b+t;

		clamp<int8_t>(r, 0, 255);
		clamp<int8_t>(g, 0, 255);
		clamp<int8_t>(b, 0, 255);

		return Color(r, g, b);
	}

	ImVec4 color_to_imvec4(const Color& color) {
		return ImVec4(
				static_cast<float>(color.r)/255.0f,
				static_cast<float>(color.g)/255.0f,
				static_cast<float>(color.b)/255.0f,
				static_cast<float>(color.a)/255.0f);
	}

	Color imvec4_to_color(const ImVec4& v4) {
		return Color(
				static_cast<uint8_t>(v4.x*255.0f),
				static_cast<uint8_t>(v4.y*255.0f),
				static_cast<uint8_t>(v4.z*255.0f),
				static_cast<uint8_t>(v4.w*255.0f));
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
	
	void add_plane_data(std::vector<Vertex>& out, const int t_settings) {
		std::vector<Vertex> tmp = [t_settings]() {
			if(t_settings & DOUBLE_SIDE) {
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
		// TODO: read with time:
		// https://math.wikia.org/wiki/Icosahedron
		// https://en.wikipedia.org/wiki/Icosahedron
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
		if(out.empty()) { return; }

		Vec3 triangle[3];

		const size_t size = out.size();
		for(size_t i = 0; i < size; i+=3) {
			if(i+2 >= size) { break; }
			triangle[0] = out[i].point;
			triangle[1] = out[i+1].point;
			triangle[2] = out[i+2].point;

			Vec3 normal = cross(triangle[1]-triangle[0], triangle[2]-triangle[0]);	
			normal = (normal/normal.length()).invert();

			out[i].normal = normal;
			out[i+1].normal = normal;
			out[i+2].normal = normal;
		}
	}

	uint32_t color_to_hex(const Color& color) {
		return (color.r<<16)|(color.g<<8)|color.b;
	}

	Color hex_to_color(const uint32_t hex) {
		return Color(hex/0x10000, (hex/0x100)%0x100, hex%0x100);
	}

	bool is_hex_string(const std::string& t_str) {
		const size_t find_result = t_str.find_first_not_of("0123456789ABCDEFabcdefxX");
		return (find_result == std::string::npos) && !t_str.empty();
	}


	namespace ImGuiExt {
	
		bool ColorPicker(const char* label, ImVec4& color, const int flags) {
			return ImGui::ColorPicker4(label, (float*)&color.x, flags | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar);
		}

		bool ColorEdit(const char* label, ImVec4& color, const int flags) {
			return ImGui::ColorEdit4(label, (float*)&color.x, flags | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar);
		}

		bool TextEdit(const char* label, std::string* str, const ImVec2& size, const int flags) {

			auto callback = [](ImGuiInputTextCallbackData* data) {
				if(data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
					std::string* str = (std::string*)data->UserData;
					str->resize(data->BufSize);
					data->Buf = (char*)str->c_str();
				}
				return 0;
			};

			return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity(), size, 
					flags | ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CallbackResize, callback, (void*)str);
		}
	}

}






