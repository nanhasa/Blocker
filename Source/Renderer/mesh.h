#pragma once

#include <vector>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

// Vertex object
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvCoord;

	bool isEqual(const Vertex& rhs) const
	{
		return position == rhs.position 
			&& normal == rhs.normal
			&& uvCoord == rhs.uvCoord;
	}
};

class Mesh {
public:

	/**
	 * \brief Constructor
	 * \param vertices Mesh vertice data
	 * \param indices Mesh indice data to vertices
	 */
	Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned short>&& indices);

	/**
	 * \brief Destructor. Deletes buffer objects
	 */
	~Mesh();

	/**
	 * \brief draw Used to draw the mesh
	 */
	void draw() const;

private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	std::vector<Vertex> m_vertices;			//!< Vertice data
	std::vector<unsigned short> m_indices;	//!< Indices to vertices

	/**
	 * \brief Used to bind mesh to OpenGL
	 */
	void setupMesh();
};
