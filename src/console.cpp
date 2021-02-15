#include "libs/imgui/imgui.h"
#include "console.hpp"

namespace vpanic {

	void Console::update() {
		ImGui::Begin("Console");
	
		ImVec2 size = ImGui::GetWindowSize();

		ImGui::BeginChild("scoll", ImVec2(size.x-20, size.y-57), ImGuiWindowFlags_HorizontalScrollbar);
		for(int i = 0; i < m_text.size(); i++) {
			ImGui::Text(m_text[i].first);
		}
		ImGui::EndChild();

		// TODO: input box

		ImGui::End();
	}

	
	void Console::add(const char* t_text, ...) {
	}

	void Console::add(const char* t_text, const Color& t_color, ...) {
	}
	
}









