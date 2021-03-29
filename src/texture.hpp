#pragma once
#include <vector>

namespace vpanic {

	class Texture {
	public:

		Texture();
		~Texture();

		// TODO: settings for loading texture and flipping it
		bool load(const char* t_filename);
		bool load_cube(const std::vector<const char*>& t_filenames);
		
		void unload();
		bool is_loaded() const;
		uint8_t get_type() const;
		uint32_t get_id() const;

		void enable();
		void disable();

	private:

		bool _generate_image(const char* t_filename, const int t_type);
		bool m_loaded { false };
		uint32_t m_id { 0 };
		int m_type { 0 };

	};

}
