#ifndef Ludum_Graphics_Mesh_hpp
#define Ludum_Graphics_Mesh_hpp

#include <Ludum/Graphics/Vertex.hpp>
#include <Ludum/Graphics/Program.hpp>
#include <Ludum/Graphics/Material.hpp>

#include <SFML/OpenGL.hpp>

#include <vector>
#include <string>

namespace gb
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(std::vector<Vertex> vertices, std::vector<int> indices);

		void addVertices(std::vector<Vertex> vertices, std::vector<int> indices);
		void draw(gb::Program* shaderProgram);

		GLuint getVBO() const { return m_vbo; }
		GLuint getIBO() const { return m_ibo; }
		GLuint getSize() const { return m_size; }

	private:
		GLuint m_vbo; // Vertex Buffer Object
		GLuint m_ibo; // Index Buffer Object
		GLuint m_size; // Number of indices

	};
}

#endif
