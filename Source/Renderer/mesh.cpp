#include "Renderer/mesh.h"

#include <3rdParty/GL/glew.h>

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices)
	: vertices(vertices), indices(indices), m_VAO(0), m_VBO(0), m_EBO(0)
{
	setupMesh();
}

Mesh::~Mesh() 
{
	//glDeleteBuffers(1, &m_EBO);
	//glDeleteBuffers(1, &m_VBO);
	//glDeleteVertexArrays(1, &m_VAO);
}

//Mesh & Mesh::operator=(Mesh && rhs) noexcept
//{
//	vertices = std::move(rhs.vertices);
//	indices = std::move(rhs.indices);
//	m_VAO = std::move(rhs.m_VAO);
//	m_VBO = std::move(rhs.m_VBO);
//	m_EBO = std::move(rhs.m_EBO);
//	return *this;
//}

void Mesh::draw() const
{
	// draw mesh
	glBindVertexArray(m_VAO);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	// Generate buffer object ids
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	// Bind objects
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvCoord));

	// Unbind
	glBindVertexArray(0);
}
