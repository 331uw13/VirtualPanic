#pragma once
#include <cstdint>

struct ImVec2;

namespace ImGui {
	namespace VPanic {

		// TODO: move console here ?
	
		// Example: "<00FF00>this text is green <0000FF>but this is blue!"
		bool TextRGB(const char* text, ...);
		
		// TODO
		bool TextEditor(const char* label, char* buf, int buf_size, const ImVec2& size_arg);

		// TODO:
		// bool ColorEdit();
		// bool ColorPicker();

	}
}
