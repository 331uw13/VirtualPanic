#pragma once
#include <chrono>
#include <cstdint>

namespace vpanic {

	typedef std::chrono::steady_clock::time_point time_point;

	class Timer {
	public:

		void reset();

		uint32_t elapsed_s();
		uint32_t elapsed_ms();
		uint64_t elapsed_ns();

		Timer() : m_point(_now()) {}
		~Timer() {}

	private:

		time_point m_point;
		time_point _now();

	};

}
