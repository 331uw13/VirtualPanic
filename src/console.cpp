//#include <cstring>
//#include <string>
#include <vector>
#include <stdarg.h>
#include <cmath>

#include "libs/imgui/imgui.h"
#include "console.hpp"
#include "utils.hpp"


static std::vector<const char*> g_buffer;
static char g_input_buffer[1024];
static int g_scroll { 0 };
static vpanic::ConsoleState g_state;


namespace vpanic {

	void console::render() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 0.0f));
		
		ImGui::Begin("Console", (bool*)nullptr, ImGuiWindowFlags_NoScrollbar);
		ImVec2 size = ImGui::GetWindowSize();
		
		ImGui::BeginChild("##console_scroll_region", ImVec2(size.x, size.y-50), true);
		const ImGuiStyle& style = ImGui::GetStyle();
		const ImGuiIO& io = ImGui::GetIO();
		
		// TODO: 
		// -  when clicked on "scroll region" set focus to input box
		// -  if mouse is hovered on text color its background to some other color?

		// update size because of "scroll region"
		size = ImGui::GetWindowSize();

		const uint32_t max_size = g_buffer.size();
		g_scroll -= static_cast<int>(io.MouseWheel);
		clamp<int>(g_scroll, 0, max_size);
	
		int iend = g_scroll+(size.y/ImGui::GetTextLineHeight());
		clamp<int>(iend, g_scroll, max_size);

		for(int i = g_scroll; i < iend; i++) {
			ImGuiExt::TextRGB(g_buffer[i]);
		}
		

		ImGui::EndChild();

        ImGui::SetNextItemWidth(size.x-30);
		if(ImGui::InputText("##input", g_input_buffer, 1024, ImGuiInputTextFlags_EnterReturnsTrue)) {
			//g_buffer.push_back(strdup(g_input_buffer));
			memset(g_input_buffer, 0, 1024);
			g_state.set(ConsoleState::FOCUS_INPUT_BOX);
		}

		// Focus for input box can be set from anywhere and not just when pressing enter or something.
		// Example if you want to focus console input box when opening it
		if(g_state[ConsoleState::FOCUS_INPUT_BOX]) {
			ImGui::SetItemDefaultFocus();
			ImGui::SetKeyboardFocusHere(-1);
			g_state.unset(ConsoleState::FOCUS_INPUT_BOX);
		}

		ImGui::End();
		ImGui::PopStyleVar(3);
	}

	void console::clear() {
		g_buffer.clear();
	}

	ConsoleState console::copy_state() {
		return g_state;
	}

	ConsoleState& console::state() {
		return g_state;
	}

	void console::print(const char* t_fmt, ...) {
		va_list a;
		va_start(a, t_fmt);
		char buf[1024];
		vsnprintf(buf, sizeof(buf), t_fmt, a);
		buf[sizeof(buf)-1] = '\0';
		va_end(a);
		g_buffer.push_back(strdup(buf));
	}

	/*
	void Console::update() {
		ImGui::Begin("Console", (bool*)NULL, ImGuiWindowFlags_NoScrollbar);
		ImVec2 size = ImGui::GetWindowSize();


		ImGui::BeginChild("##scoll", ImVec2(size.x-20, size.y-67), false, ImGuiWindowFlags_HorizontalScrollbar);
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

		// focus for input text can be set from anywhere not just when pressing enter or something
		if(m_input_needs_focus) {
			ImGui::SetItemDefaultFocus();
			ImGui::SetKeyboardFocusHere(-1);
			m_input_needs_focus = false;
		}

		//ImGui::SetScrollHereY(1.0f);

		ImGui::End();
	}


	// TODO: rewrite printting... xdd

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
	
	void Console::callback(void(*t_callback)(const std::vector<std::string>&)) {
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
	*/


}


