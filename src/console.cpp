#include <stdarg.h>
#include <cmath>
#include <unordered_map>

#include "libs/imgui/imgui.h"
#include "console.hpp"
#include "utils.hpp"
#include "settings.hpp"

static std::vector<const char*> g_buffer;
static std::unordered_map<std::string, void(*)(const std::vector<std::string>&)> g_commands;
static char g_input_buffer[1024];
static int g_scroll { 0 };
static vpanic::State g_state;


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
			std::string_view view(g_input_buffer);
	
			if(g_commands[std::string(view.substr(0, view.find(0x20)))]) {
				size_t end = 0;
				size_t start = 0;

				std::vector<std::string> out;
				while((start = view.find_first_not_of(0x20, end)) != std::string_view::npos) {
					end = view.find(0x20, start);
					out.push_back(std::string(view.substr(start, end-start)));
				}
			
				g_commands[out[0]](out);
			}
			else {
				print("<%x>[ERROR] --> Unknown command \"%s\"", MSG_ERROR_COLOR, g_input_buffer);
			}
			
			memset(g_input_buffer, 0, 1024);
			g_state.set(CONSOLE_GET_FOCUS, 1);
		}

		// Focus for input box can be set from anywhere and not just when pressing enter or something.
		// Example if you want to focus console input box when opening it
		
		if(g_state[CONSOLE_GET_FOCUS]) {
			ImGui::SetItemDefaultFocus();
			ImGui::SetKeyboardFocusHere(-1);
			g_state.set(CONSOLE_GET_FOCUS, 0);
		}
		

		ImGui::End();
		ImGui::PopStyleVar(3);
	}
	
	void console::add_command(const char* t_str, void(*t_callback)(const std::vector<std::string>&)) {
		g_commands[t_str] = t_callback;
	}

	void console::clear() {
		g_buffer.clear();
	}

	State console::copy_state() {
		return g_state;
	}

	State& console::state() {
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

}


