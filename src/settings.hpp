#pragma once

namespace VPanic {

	
	// --- 'Engine::init()' ---

	// use this if you want to use imgui too.
	// not default
	static constexpr int INIT_IMGUI { 1 << 0 };

	// window mode will be set to fullscreen after engine is ready to use.
	// not default
	static constexpr int FULLSCREEN { 1 << 1 };

	// vertical sync will be used when engine is ready to use.
	// not default
	static constexpr int VSYNC { 1 << 2 };

	// dont call 'render_back(false)' on init
	// not default
	static constexpr int NO_FACE_CULLING { 1 << 3 };


	// --- 'Engine::winding_order()' ---
	
	static constexpr int CLOCKWISE { 1 };
	static constexpr int COUNTER_CLOCKWISE { 2 };



	// --- 'Shape::load()' ---

	// dont pass normals for shaders
	static constexpr int NO_COLORED_LIGHT { 1 };


	// --- 'Shader::load()' ---

	// this will not add any util functions for shaders.
	// not default
	static constexpr int NO_SHADER_UTIL { 1 << 0 };

	// this will give you support for geometry shaders.
	// TODO: static constexpr int SUPPORT_GEOMETRY_SHADER { 1 << 1 };


	// ...


}

