#pragma once
#include <chrono>
#include <cstdint>

typedef std::chrono::steady_clock::time_point chrono_tp;

namespace VPanic {

	class Timer {
	public:

		void reset();

		uint32_t elapsed_s();
		uint32_t elapsed_ms();
		uint64_t elapsed_ns();

		Timer() : m_point(_now()) {}
		~Timer() {}

	private:

		chrono_tp m_point;
		chrono_tp _now();

	};

}
