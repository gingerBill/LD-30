#include <Ludum/Graphics/Mesh.hpp>

#include <Ludum/Common.hpp>

#include <algorithm>

namespace gb
{
	namespace
	{
	}

	Mesh::Mesh()
		:m_vbo(0)
		,m_ibo(0)
	{
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);
		m_size = 0;
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices)
		:Mesh()
	{
		addVertices(vertices, indices);
	}

	void Mesh::addVertices(std::vector<Vertex> vertices, std::vector<int> indices)
	{
		std::reverse(indices.begin(), indices.end());

		m_size = indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER,
		             vertices.size() * Vertex::SIZE * sizeof(float),
		             &vertices[0],
		             GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		             indices.size() * sizeof(int),
		             &indices[0],
		             GL_STATIC_DRAW);

		// Unbind VBO and IBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Mesh::draw(gb::Program* shaderProgram)
	{
		glEnableVertexAttribArray(shaderProgram->attrib("vert"));
		glEnableVertexAttribArray(shaderProgram->attrib("texCoord"));

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(shaderProgram->attrib("vert"),
		                      3,
		                      GL_FLOAT,
		                      false,
		                      Vertex::SIZE * sizeof(float),
		                      (const GLvoid*)(0 * sizeof(float)));

		glVertexAttribPointer(shaderProgram->attrib("texCoord"),
		                      2,
		                      GL_FLOAT,
		                      false,
		                      Vertex::SIZE * sizeof(float),
		                      (const GLvoid*)(3 * sizeof(float)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glDrawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, 0);

		// Unbind VBO and IBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(shaderProgram->attrib("vert"));
		glDisableVertexAttribArray(shaderProgram->attrib("texCoord"));
	}
}
