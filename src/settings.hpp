#pragma once

namespace vpanic {

	
	// --- 'Engine::init()' ---
	
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
	// not default
	static constexpr int NO_COLORED_LIGHT { 1 << 0 };

	// --- 'Shape::disable_outline()' ---
	// unloads the current outline shader if loaded
	// by default it sets the outline shader to nullptr but doesnt unload it
	static constexpr int UNLOAD_ON_DISABLE { 1 };


	// --- 'Shader::load()' ---
	// this will not add any util functions for shaders.
	// not default
	static constexpr int NO_SHADER_UTIL { 1 };

	// this will give you support for geometry shaders.
	// TODO: static constexpr int SUPPORT_GEOMETRY_SHADER { 1 << 1 };


	// --- 'add_plane_data()' ---
	// do you need to see both sides of the plane?
	// not default
	static constexpr int DOUBLE_SIDE { 1 };


	// --- 'set_normals()' ---
	static constexpr int SMOOTH_NORMALS { 1 };

	// --- 'find_all()' ---
	// dont ignore sub directories
	// not default
	static constexpr int INCLUDE_SUB_DIRECTORIES { 1 };

	// ...


}

