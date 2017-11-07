#include "Renderer/model.h"

Model::Model(std::vector<Mesh>&& meshes) 
	: m_meshes(std::forward<std::vector<Mesh>>(meshes)) {}

void Model::draw(const ShaderProgram & shader, const unsigned int textureId) const
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	shader.use();
	for (unsigned int i = 0; i < m_meshes.size(); ++i) {
		m_meshes[i].draw();
	}
}
