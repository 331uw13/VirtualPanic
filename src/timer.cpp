#include <chrono>
#include "timer.hpp"

namespace vpanic {

	void Timer::reset() {
		m_point = std::chrono::steady_clock::now();
	}

	uint32_t Timer::elapsed_s() {
		return std::chrono::duration_cast<std::chrono::seconds>(_now() - m_point).count();
	}
	
	uint32_t Timer::elapsed_ms() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(_now() - m_point).count();
	}
	
	uint64_t Timer::elapsed_ns() {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(_now() - m_point).count();
	}
	
	time_point Timer::_now() {
		return std::chrono::steady_clock::now();
	}

}

