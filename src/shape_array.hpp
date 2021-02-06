#pragma once
#include <glm/vec3.hpp>
#include <vector>

#include "shader.hpp"

namespace VPanic {

	class ShapeArray {
	public:

		void place(const std::vector<glm::vec3>& t_positions);
		void set(const glm::vec3& t_pos, uint32_t t_index);
		void reserve(const uint32_t t_size);
		uint32_t size() const;
		uint32_t count() const;

		void load(const std::vector<float>& t_data);
		void unload();
		bool is_loaded() const;

		void draw(const Shader& t_shader) const;
		
		ShapeArray(const std::vector<float>& t_data);
		ShapeArray() {}
		~ShapeArray();

	private:

		bool m_loaded       { false };
		uint32_t m_vao      { 0 };
		uint32_t m_vbo      { 0 };
		uint32_t m_ibuffer  { 0 };
		uint32_t m_count    { 0 };
		uint32_t m_draw_data_size { 0 };
		uint32_t m_reserved { 0 };
	};

}
