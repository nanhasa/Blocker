#pragma once

#include <vector>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvCoord;
};

class Mesh {
public:

	/**
	 * \brief Mesh
	 * \param vertices
	 * \param indices
	 */
	Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices);

	/**
	 * \brief Mesh
	 */
	~Mesh();

	//Mesh(Mesh const&) = delete;
	//Mesh& operator=(const Mesh& rhs) = delete;
	//Mesh& operator=(Mesh&& rhs) noexcept;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	/**
	 * \brief draw
	 */
	void draw() const;

private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	/**
	 * \brief setupMesh
	 */
	void setupMesh();
};
