#include <stdarg.h>
#include <cstring>

#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_internal.h"

#include "imgui_extension.hpp"
#include "color.hpp"
#include "utils.hpp"


bool ImGui::VPanic::TextRGB(const char* text, ...) {
	
	ImGuiWindow* window = GetCurrentWindow();
	
	char buf[600];
	va_list ap;
	va_start(ap, text);
	vsnprintf(buf, sizeof(buf), text, ap);
	buf[sizeof(buf)-1] = '\0';
	va_end(ap);

	char text_buffer[1024];
	char color_buffer[32];

	uint32_t text_index = 0;
	uint32_t color_index = 0;

	bool color_was_set = false;
	bool write_color = false;
	const uint32_t len = strlen(buf);

	float cursor_y = window->DC.CursorPos.y;
	float cursor_x = window->DC.CursorPos.x;

	auto text_sameline = [&](const char* t_text_part) {
		const char* text_end = t_text_part+strlen(t_text_part);
		const ImVec2 text_pos(cursor_x, cursor_y);
		const ImVec2 text_size = CalcTextSize(t_text_part, text_end, false, 0);

		ImRect bb(text_pos, ImVec2(text_pos.x+text_size.x, text_pos.y+text_size.y));
		ItemSize(text_size, 0.0f);
		if (!ItemAdd(bb, 0)) {
			return;
		}
    
		RenderTextWrapped(bb.Min, t_text_part, text_end, 0);

		cursor_x += text_size.x;
	};


	for(uint32_t i = 0; i < len; i++) {
		
		if(buf[i] == '>') {
			write_color = false;
			if(color_index > 0) {
				color_buffer[color_index] = '\0';
				const bool can_use_color = vpanic::is_hex_string(color_buffer);

				if(can_use_color) {
					if(color_was_set) {
						PopStyleColor();
					}
					PushStyleColor(ImGuiCol_Text, vpanic::color_to_imvec4(vpanic::hex_to_color(std::stoi(color_buffer, nullptr, 16))));
					color_was_set = true;
				}

				strcpy(color_buffer, ""); // yes, all color values should be the same length but just to be more safe
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

	}

	// if there was no color at the end (probably not) so it needs to render the rest too
	if(text_index > 0) {
		text_buffer[text_index] = '\0';
		text_sameline(text_buffer);
	}

	if(color_was_set) {
		PopStyleColor();
	}

	return true;
}


bool ImGui::VPanic::TextEditor(const char* label, char* buf, int buf_size, const ImVec2& size_arg) {


	return false;
}





