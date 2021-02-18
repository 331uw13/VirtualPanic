#include <cstring>
#include <string>
#include <stdarg.h>

#include "libs/imgui/imgui.h"

#include "console.hpp"
#include "utils.hpp"


namespace vpanic {

	void Console::update() {
		ImGui::Begin("Console");
		ImVec2 size = ImGui::GetWindowSize();



		ImGui::BeginChild("##scoll", ImVec2(size.x-20, size.y-58), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
		
		float old_font_size = ImGui::GetFont()->Scale;
		ImGui::SetWindowFontScale(m_font_size);

		//Console& inst = get_instance();

		for(size_t i = 0; i < m_text.size(); i++) {
			ImGui::TextColored(color_to_imvec4(m_text[i].second), m_text[i].first);
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
		}
	
		ImGui::PopStyleVar();
		ImGui::SetWindowFontScale(old_font_size);
		ImGui::EndChild();
		
        ImGui::SetNextItemWidth(size.x-30);
		if(ImGui::InputText("##input", m_input_buffer, 1024, ImGuiInputTextFlags_EnterReturnsTrue)) {
			const char* d = strdup(m_input_buffer);
			_exec(d);
			strcpy(m_input_buffer, "");
			
			//ImGui::SetItemDefaultFocus();
			//ImGui::SetKeyboardFocusHere(-1);
			m_input_needs_focus = true;
		}

		// so focus for input text can be set from anywhere not just when pressing enter or something
		if(m_input_needs_focus) {
			ImGui::SetItemDefaultFocus();
			ImGui::SetKeyboardFocusHere(-1);
			m_input_needs_focus = false;
		}

		ImGui::SetScrollHereY(1.0f);

		ImGui::End();
	}

	void Console::print(const char* t_text, ...) {
		char buf[600];
		va_list ap;
		va_start(ap, t_text);
		vsnprintf(buf, sizeof(buf), t_text, ap);
		buf[sizeof(buf)-1] = '\0';
		va_end(ap);

		m_text.push_back(std::make_pair(strdup(buf), Color(180, 180, 180)));
	}

	void Console::print(const Color& t_color, const char* t_text, ...) {
		char buf[600];
		va_list ap;
		va_start(ap, t_text);
		vsnprintf(buf, sizeof(buf), t_text, ap);
		buf[sizeof(buf)-1] = '\0';
		va_end(ap);

		m_text.push_back(std::make_pair(strdup(buf), t_color));
	}
	
	void Console::clear_text() {
		m_text.clear();
	}
	
	void Console::set_callback(void(*t_callback)(const std::vector<std::string>&)) {
		m_callback = t_callback;
	}
	
	float Console::get_font_size() const {
		return m_font_size;
	}
	
	void Console::set_font_size(const float t_size) {
		m_font_size = t_size;
		clamp<float>(m_font_size, 0.1f, 5.0f);
	}
	
	void Console::_exec(const char* t_cmd) {	
		std::string_view view(t_cmd);

		if(m_callback != nullptr) {	
			
			std::vector<std::string> out;

			size_t start = 0;
			size_t end = 0;

			while((start = view.find_first_not_of(0x20, end)) != std::string_view::npos) {
				end = view.find(0x20, start);
				out.push_back(std::string(view.substr(start, end-start)));
			}

			m_callback(out);
		}
		else {
			print("No callback set!", Color(200, 150, 10));
		}
	}
	
	void Console::focus() {
		m_input_needs_focus = true;
	}


}









