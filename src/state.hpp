#pragma once


namespace vpanic {

	class State {
	public:

		bool operator [] (const int t_flag) const { 
			return (m_data & t_flag);
		}

		bool is_set(const int t_flag) const {
			return (m_data & t_flag);
		}

		void set(const int t_flag, const uint8_t e) {
			e ? (m_data |= t_flag) : (m_data &= ~t_flag);
		}

		void clear() {
			m_data = 0;
		}


	private:
	
		int m_data { 0 };
	
	};

}





