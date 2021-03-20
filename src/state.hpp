#pragma once


namespace vpanic {


	struct DefaultState {
		
		bool operator [] (const int t_flag) const { 
			return (m_data & t_flag);
		}

		bool is_set(const int t_flag) const {
			return (m_data & t_flag);
		}

		void set(const int t_flag) {
			m_data |= t_flag;
		}

		void unset(const int t_flag) {
			m_data &= ~t_flag;
		}

		void clear() {
			m_data = 0;
		}

	private:
		int m_data { 0 };
	};


	struct EngineState : public DefaultState {
		enum {
			OK            = 1 << 0,
			INIT_OK       = 1 << 1,
			QUIT          = 1 << 2,
			KEEP_LOOP     = 1 << 3,
			LOCK_MOUSE    = 1 << 4,
			FULLSCREEN_ENABLED    = 1 << 5,
			VSYNC_ENABLED         = 1 << 6
		};
	};

	struct ShaderState : public DefaultState {
		enum {
			HAS_VS = 1 << 0, // vertex shader
			HAS_FS = 1 << 1, // fragment shader
			HAS_GS = 1 << 2, // geometry shader
			HAS_CS = 1 << 3  // compute shader
		};
	};


	// probably going to add more here...

}
