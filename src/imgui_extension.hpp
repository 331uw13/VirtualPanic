#pragma once
#include <cstdint>

struct ImVec2;

namespace ImGui {
	namespace VPanic {

		// TODO: move console here ?
	
		// %z  vpanic::Color(r, g, b, a)
		bool TextRGB(const char* text, ...);
		bool TextEditor(const char* label, char* buf, int buf_size, const ImVec2& size_arg);

		// TODO:
		// bool ColorEdit();
		// bool ColorPicker();

	}
}
