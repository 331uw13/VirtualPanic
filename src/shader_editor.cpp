#include "shader_editor.hpp"
#include "utils.hpp"

#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_internal.h"


namespace ImGui {

	static int TextEditCallback(ImGuiInputTextCallbackData* data) {
    	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
			std::string* str = (std::string*)data->UserData;
        	IM_ASSERT(data->Buf == str->c_str());
        	str->resize(data->BufTextLen);
        	data->Buf = (char*)str->c_str();
		}
		return 0;
	}

	bool TextEdit(const char* label, std::string* str, const ImVec2& size_arg, ImGuiInputTextFlags flags = 0) {

		flags |= ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_Multiline;
		return InputTextMultiline(label, (char*)str->c_str(), str->capacity()+1, size_arg, flags, TextEditCallback, (void*)str);
	}
}


namespace vpanic {

	void ShaderEditor::update() {
	
		// NOTE: MenuBar looks bad!
		/*
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.2f, 2.1f));
		*/

		ImGui::PushStyleColor(ImGuiCol_WindowBg, color_to_imvec4(Color(23, 23, 23)));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, color_to_imvec4(Color(23, 23, 23)));
		ImGui::PushStyleColor(ImGuiCol_Text, color_to_imvec4(Color(220, 220, 230)));
		ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, color_to_imvec4(Color(200, 60, 255)));
		//ImGui::PushStyleColor(ImGuiCol_Button, color_to_imvec4(Color(5, 10, 30)));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.2f, 0.2f));
		
		ImGui::Begin("Shader Editor", (bool*)NULL, ImGuiWindowFlags_NoScrollbar/* | ImGuiWindowFlags_MenuBar*/);

		if(m_editor_font != nullptr) {
			ImGui::PushFont(m_editor_font);
		}
		
		// this should be purple and Is this red?
		// Is this green?
		// Is this blue?
	//	ImGui::VPanic::TextRGB("<FF00FF>This should be purple and <FF0000>Is this red?\n <00FF00>Is this green?\n <0000FF>Is this blue?");
	/*	
		{ // test
			
			static char buf[1024*2] {
				"Hello? is this working?"
			};

			ImGui::VPanic::CodeEditor("##TESTING_TEXT_EDITOR", buf, 64, ImGui::VPanic::GLSL_SyntaxHighlight, ImGui::GetWindowSize());
		}
	*/	

		if(m_editor_font != nullptr) {
			ImGui::PopFont();
		}

		/*
		if(ImGui::BeginMenuBar()) {

			if(ImGui::BeginMenu("File")) {
				if(ImGui::MenuItem("Open")) {
				}
				if(ImGui::MenuItem("Save")) {
				}
				if(ImGui::MenuItem("Save as")) {
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();
		
			if(ImGui::Button("Compile")) {
			}
			
			ImGui::EndMenuBar();
		}

		ImVec2 win_size = ImGui::GetWindowSize();
		win_size.y -= 50.f;
		win_size.x -= 50.f;

		ImGui::TextEdit("##source-edit", &m_source, win_size);

		//ImGui::PopStyleVar(3);
*/
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(4);
		ImGui::End();
		
	}

	void ShaderEditor::open(Shader* t_shader_ptr) {
		m_shader = t_shader_ptr;
	}
	
	void ShaderEditor::load_font(const char* t_filename, const float t_size) {
		m_editor_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(t_filename, t_size);
	}


}
