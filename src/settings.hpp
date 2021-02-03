#pragma once

namespace VPanic {

	// 'Engine::init()'
	static constexpr int INIT_IMGUI     { 1 << 0 };
	static constexpr int FULLSCREEN     { 1 << 1 };
	static constexpr int VSYNC          { 1 << 2 };

	// 'Shader::load()'
	static constexpr int NO_SHADER_UTIL              { 1 << 0 };
	static constexpr int NO_DEFAULT_VERTEX_SHADER    { 1 << 1 };
	// TODO: static constexpr int SUPPORT_GEOMETRY_SHADER   { 1 << 2 };

}
