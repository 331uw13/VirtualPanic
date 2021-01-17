#pragma once
#include <chrono>

namespace VPanic {
	class Timer {
	public:

		void reset();
		int elapsed();

		Timer();
		~Timer() {};

	private:
		std::chrono::steady_clock::time_point m_point;	
	};
}
