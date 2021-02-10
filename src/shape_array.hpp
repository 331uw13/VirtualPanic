#pragma once
#include <glm/vec3.hpp>
#include <vector>

#include "shader.hpp"
#include "vertex.hpp"

namespace VPanic {

	class ShapeArray {
	public:

		Color color { Color(5, 240, 5) };

		void place(const std::vector<glm::mat4>& t_matrices);
		void set(const uint32_t t_index, const glm::mat4& t_matrix);
		void reserve(const uint32_t t_size);
		uint32_t size() const;

		void load(const std::vector<Vertex>& t_data, const int t_settings = 0);
		void unload();
		bool is_loaded() const;

		void draw(const Shader& t_shader) const;
	
		uint8_t get_type() const;

		ShapeArray(const std::vector<Vertex>& t_data, const int t_settings = 0);
		ShapeArray() {}
		~ShapeArray();

	private:

		bool m_loaded       { false };
		uint32_t m_vao      { 0 };
		uint32_t m_vbo      { 0 };
		uint32_t m_ibuffer  { 0 };
		uint32_t m_reserved { 0 };
		uint8_t  m_type     { 0 };
		uint32_t m_draw_data_size { 0 };

	};

}
