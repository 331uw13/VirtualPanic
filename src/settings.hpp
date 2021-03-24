#pragma once

namespace vpanic {

	
	// --- 'Engine::init()' ---
	
	// window mode will be set to fullscreen after engine is ready to use.
	// not default
	static constexpr int FULLSCREEN { 1 << 0 };

	// vertical sync will be used when engine is ready to use.
	// not default
	static constexpr int VSYNC { 1 << 1 };


	// --- 'Engine::winding_order()' ---
	static constexpr int CLOCKWISE { 1 };
	static constexpr int COUNTER_CLOCKWISE { 2 };

	// --- 'add_plane_data()' ---
	// do you need to see both sides of the plane?
	// not default
	static constexpr int DOUBLE_SIDE { 1 << 0 };
	static constexpr int USING_TRIANGLE_STRIP { 1 << 1 };

	// --- 'find_all()' ---
	// dont ignore sub directories
	// not default
	static constexpr int INCLUDE_SUB_DIRECTORIES { 1 };


	// ...


}

