#pragma once
#include <cstdint>

namespace vpanic {

	class Texture {
	public:

		Texture(const char* t_filename);
		Texture() {}
		~Texture();

		// TODO: settings for loading texture and flipping it
		bool load(const char* t_filename);
		void unload();
		bool is_loaded() const;

		void use();

	private:

		bool m_loaded { false };
		uint32_t m_id { 0 };

	};

}
