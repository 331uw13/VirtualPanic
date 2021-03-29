#pragma once
#include <vector>

#include "vec2.hpp"


namespace vpanic {

	class Framebuffer {
	public:

		void create(const uint32_t t_colattm_count, const Vec2& t_frame_size);
		void destroy();

		bool is_created() const;
		uint32_t id() const;
		uint32_t colattm_id(const uint32_t t_indx) const;

	private:

		std::vector<uint32_t> m_attachments;
		uint32_t m_id { 0 };
		uint32_t m_rbo { 0 };
		bool m_created { false };

	};

}
