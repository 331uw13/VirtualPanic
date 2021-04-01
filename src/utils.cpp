#include <cmath>
#include <vector>
#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_internal.h"

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
		return min+static_cast<float>(fast_rand())/static_cast<float>((float)0x7FFF/(float)(max-min));
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
	
	Color random_color(const uint8_t min, const uint8_t max) {
		return Color(
					static_cast<uint8_t>(random(min, max)),
					static_cast<uint8_t>(random(min, max)),
					static_cast<uint8_t>(random(min, max)),
					255);
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
	
	void set_plane_data(std::vector<Vertex>& out, const int t_settings) {
		out = [t_settings]() {
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
					Vertex(-0.5f,  0.5f,  0.0f),
					Vertex( 0.5f,  0.5f,  0.0f),	
					Vertex( 0.5f,  0.5f,  0.0f),
					Vertex( 0.5f, -0.5f,  0.0f),
					Vertex(-0.5f, -0.5f,  0.0f),
				};
			}
		}();
		set_texcoords(out);
	}

	void set_sphere_data(std::vector<Vertex>& out) {
		// TODO: read with time:
		// https://math.wikia.org/wiki/Icosahedron
		// https://en.wikipedia.org/wiki/Icosahedron
	}
	
	void set_box_data(std::vector<Vertex>& out) {
		out = {
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
	}

	void set_texcoords(std::vector<Vertex>& out, const float t) {
		if(out.size() < 6) { return; }

		for(size_t i = 0; i < out.size()-5; i+=6) {
			for(uint32_t j = 0; j < 6; j++) {
				Vertex& p = out[i+j];
				p.texcoords.x = (p.point.x+0.5f)*t;
				p.texcoords.y = (p.point.y+0.5f)*t;
			}
		}

	}

	void set_normals(std::vector<Vertex>& out) {	
		foreach_triangle(out, [](Vertex& p0, Vertex& p1, Vertex& p2) {
				Vec3 normal = cross(p1.point-p0.point, p2.point-p0.point);
				normal = (normal/normal.length()).invert();
				p0.normal = normal;
				p1.normal = normal;
				p2.normal = normal;
				});
	}

	void foreach_triangle(std::vector<Vertex>& vertices, const std::function<void(Vertex& p0, Vertex& p1, Vertex& p2)> f) {
		if(vertices.size() < 3) { return; }
		for(size_t i = 0; i < vertices.size()-2; i+=3) {
			f(vertices[i], vertices[i+1], vertices[i+2]);
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

	Shader create_default_image_shader() {
		Shader shader;

		const char* const fs =
			"#version 430\n"
			"uniform sampler2D texture0;"
			"in vec2 img_texcoord;"
			"out vec4 out_color;"
			"void main() {"
				" out_color = texture(texture0, texcoord);"
			"}";

		const char* const vs = 
			"#version 430\n"
			"layout(location = 0) in vec2 pos;\n"
			"layout(location = 1) in vec2 texcoord;\n"
			"out vec2 img_texcoord;"
			"void main() {"
				" gl_Position = pos;"
				" im_texcoord = texcoord;"
			"}";

		shader.add_src_from_memory(vs, VERTEX_SHADER);
		shader.add_src_from_memory(fs, FRAGMENT_SHADER);
		shader.compile();
		return shader;
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

		void TextRGB(const char* fmt, ...) {
			if(fmt == nullptr) { return; }

			ImGuiWindow* window = ImGui::GetCurrentWindow();

			const uint32_t max_buf_size = 2048;

			char buf[max_buf_size];
			va_list ap;
			va_start(ap, fmt);
			vsnprintf(buf, sizeof(buf), fmt, ap);
			buf[sizeof(buf)-1] = '\0';
			va_end(ap);

			char text_buffer[max_buf_size];

			uint32_t text_index = 0;
			uint32_t color_count = 0;

			std::string_view viewb(buf);
			const uint32_t buf_length = strlen(buf);
			
			ImVec2 pos(window->DC.CursorPos.x, window->DC.CursorPos.y+window->DC.CurrLineTextBaseOffset);
			ImRect text_rect(pos, pos);

			auto _enable_color = [&](const uint32_t i) {
				uint32_t color_size = 0;
				const uint32_t end_pos = viewb.find('>', i);
				if(end_pos != std::string_view::npos) {
					const uint32_t start_pos = i+1;
					const uint32_t len = end_pos-start_pos;
					color_size = len+1;
					
					char* colorstr = (char*)viewb.substr(start_pos, len).data();
					colorstr[len] = '\0';
					
					if(is_hex_string(colorstr)) {
						if(start_pos+len < buf_length) {
							if(color_count > 0) {
								ImGui::PopStyleColor();
							}

							//uint32_t color = convert_str<uint32_t>(colorstr);
							//color |= 24 << 255;
							//ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(color));

							ImGui::PushStyleColor(ImGuiCol_Text, color_to_imvec4(hex_to_color(convert_str<uint32_t>(colorstr))));
							color_count++;
						}
					}
					// TODO: "</>" resets color
				}

				return color_size;
			};

			auto _render_text = [&]() {
				if(text_index == 0 || text_index >= max_buf_size) { return; }
				text_buffer[text_index] = '\0';
				const uint32_t text_length = strlen(text_buffer);
				const char* text_buffer_end = text_buffer+text_length;
				const ImVec2 text_size = ImGui::CalcTextSize(text_buffer, text_buffer_end, false, 0.0f);

				//float line_height = ImGui::GetTextLineHeight();
				//float off = s.ItemSpacing.y;
				
				ImGui::RenderText(pos, text_buffer, text_buffer_end);

				pos.x += text_size.x;
				text_rect.Max = text_size;
				memset(text_buffer, 0, text_index);
				text_index = 0;
			};


			for(uint32_t i = 0; i < buf_length; i++) {
				const char ch = buf[i];
				switch(ch) {

					// start of color tag
					case '<':
						_render_text();
						i += _enable_color(i);
						break;
						
						
					case '\n':
						pos.y += ImGui::GetTextLineHeight();
						break;
						

					default:
						text_buffer[text_index] = ch;
						text_index++;
						break;
				}
			}
			
			ImGui::ItemSize(ImVec2(0, ImGui::GetTextLineHeight()), 0.0f);
			ImGui::ItemAdd(text_rect, 0);

			_render_text();

			if(color_count > 0) {
				ImGui::PopStyleColor();
			}
			
				
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			/*
			bool pop_color = false;
			bool write_color = false;
			const uint32_t len = strlen(buf);

			float cursor_y = window->DC.CursorPos.y;
			float cursor_x = window->DC.CursorPos.x;
			float old_cursor_x = cursor_x;

			bool had_newline = false;

			auto text_sameline = [&](const char* t_text_part) {
				const char* text_end = t_text_part+strlen(t_text_part);
				const ImVec2 text_pos(cursor_x, cursor_y);
				ImVec2 text_size = ImGui::CalcTextSize(t_text_part, text_end);
				ImRect bb(text_pos, ImVec2(text_pos.x+text_size.x, text_pos.y+text_size.y));
				ImGui::ItemSize(ImVec2(text_size.x,
						   	g.FontSize/2+ImGui::GetStyle().ItemSpacing.y), 0.0f);
				ImGui::ItemAdd(bb, 0);
				ImGui::RenderText(text_pos, t_text_part, text_end);
				
				if(had_newline) {
					cursor_x = old_cursor_x;
					cursor_y += text_size.y;
					had_newline = false;
				}
				else {
					cursor_x += text_size.x;
				}
			};

			for(uint32_t i = 0; i < len; i++) {
				if(buf[i] == '>') {
					write_color = false;
					if(color_index > 0) {
						color_buffer[color_index] = '\0';
						const bool can_use_color = is_hex_string(color_buffer);

						if(pop_color) {
							ImGui::PopStyleColor();
						}
						if(can_use_color) {
							ImGui::PushStyleColor(ImGuiCol_Text, color_to_imvec4(hex_to_color(std::stoi(color_buffer, nullptr, 16))));
							pop_color = true;
						}
						else if(color_buffer[0] == '/') { // "</>" sets back to default color
							ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_Text));
							pop_color = true;
						}

						strcpy(color_buffer, ""); // yes, all color values should be the same length
						color_index = 0;
					}
				}
				else if(buf[i] == '<') {
					write_color = true;
					if(text_index > 0) {
						text_buffer[text_index] = '\0';
						text_sameline(text_buffer);
						strcpy(text_buffer, "");
						text_index = 0;
					}
				}
				else if(write_color) {
					color_buffer[color_index] = buf[i];
					color_index++;
				}
				else {
					text_buffer[text_index] = buf[i];
					text_index++;
				}

				if(buf[i] == '\n') {
					had_newline = true;
				}

			}

			// if there was no color at the end (probably not), it needs to render the rest
			if(text_index > 0) {
				had_newline = true;
				text_buffer[text_index] = '\0';
				text_sameline(text_buffer);
			}

			if(pop_color) {
				ImGui::PopStyleColor();
			}
			*/
		}
	}

}






