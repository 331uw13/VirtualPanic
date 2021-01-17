#include <chrono>

#include "timer.hpp"

namespace VPanic {

	Timer::Timer() : m_point(std::chrono::steady_clock::now()) {}

	void Timer::reset() {
		m_point = std::chrono::steady_clock::now();
	}

	int Timer::elapsed() {
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::steady_clock::now() - m_point).count();
	}

}

