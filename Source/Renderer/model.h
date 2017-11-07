#pragma once

#include "Renderer/mesh.h"
#include "Renderer/shaderprogram.h"

class Model {
public:

	/**
	 * \brief Constructor
	 * \param meshes Model meshes that consist of vertices
	 */
	explicit Model(std::vector<Mesh>&& meshes);

	~Model() = default;

	/**
	 * \brief draw Used to draw the meshes of the model
	 * \param shader Reference to shader used
	 * \param textureId	OpenGL id to texture
	 */
	void draw(const ShaderProgram& shader, const unsigned int textureId) const;

private:
	std::vector<Mesh> m_meshes;	//!< Meshes of the model
};
