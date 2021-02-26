#pragma once
#include <cstdint>

struct ImVec2;

namespace ImGui {
	namespace VPanic {

		// TODO: move console here ?
	
		// Example: "<00FF00>This part of the text is green<FF0000> and this should be red!"
		bool TextRGB(const char* text, ...);
		
		bool TextEditor(const char* label, char* buf, int buf_size, const ImVec2& size_arg);

		// TODO:
		// bool ColorEdit();
		// bool ColorPicker();

	}
}
