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


	// --- 'Shader::load()' ---

	// this will not add any util functions for shaders.
	// not default
	static constexpr int NO_SHADER_UTIL { 1 << 0 };

	// this will give you support for geometry shaders.
	static constexpr int SUPPORT_GEOMETRY_SHADER { 1 << 1 };


	// --- 'Shape::load()' && 'Shape::Shape()' ---

	// tldr: dont pass normals to shaders
	// this acheves the effect that there is no controllable light sources
	// and everything will look very flat.
	// It is good for testing when adding new things or debugging.
	// not default
	static constexpr int NO_COLORED_LIGHT { 1 << 0 };

	// set color per fragment
	static constexpr int INCLUDED_COLOR_DATA { 1 << 1 };
	
	// ...


	// --- 'Shape::draw()' ---
	
	// draw only wireframe.
	// not default
	static constexpr int WIREFRAME { 1 << 0 };

	// ...
	


}





