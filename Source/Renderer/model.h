#pragma once

#include "Renderer/mesh.h"
#include "Renderer/shaderprogram.h"

class Model {
public:

	/**
	 * \brief Model
	 * \param meshes
	 */
	explicit Model(std::vector<Mesh>&& meshes);

	/**
	 * \brief ~Model
	 */
	~Model();

	/**
	 * \brief draw
	 * \param shader
	 * \param textureId
	 */
	void draw(const ShaderProgram& shader, const unsigned int textureId) const;

private:
	std::vector<Mesh> m_meshes;
};
