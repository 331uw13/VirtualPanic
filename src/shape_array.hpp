#pragma once
#include <glm/vec3.hpp>
#include <vector>

#include "shader.hpp"
#include "vertex.hpp"

namespace VPanic {

	class ShapeArray {
	public:

		Color color { Color(240, 5, 5) };

		void place(const std::vector<glm::mat4>& t_matrices);
		void set(const uint32_t t_index, const glm::mat4& t_matrix);
		void reserve(const uint32_t t_size);
		uint32_t size() const;

		void load(const std::vector<Vertex>& t_data);
		void unload();
		bool is_loaded() const;

		void draw(const Shader& t_shader) const;
		
		ShapeArray(const std::vector<Vertex>& t_data);
		ShapeArray() {}
		~ShapeArray();

	private:

		bool m_loaded       { false };
		uint32_t m_vao      { 0 };
		uint32_t m_vbo      { 0 };
		uint32_t m_ibuffer  { 0 };
		uint32_t m_draw_data_size { 0 };
		uint32_t m_reserved { 0 };

	};

}
