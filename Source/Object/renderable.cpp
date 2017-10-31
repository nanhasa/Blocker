#include "Object/renderable.h"

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#pragma warning (pop)      // Restore back

Renderable::Renderable(ModelManager& modelManager,
	const std::string& textureFilename, const std::string& modelFilename)
{
	m_textureId = modelManager.getTextureId(textureFilename);
	m_model = modelManager.getModel(modelFilename);
}

Renderable::~Renderable() {}

void Renderable::onUpdate(IRenderer& renderer, Transform& transform) const
{
	auto trans = glm::translate(glm::mat4(), transform.position);
	trans = trans * transform.getRotationMatrix();
	const auto shader = renderer.vGetShaderProgram();
	shader->setMat4("model", trans);
	m_model->draw(*shader, m_textureId);
}
