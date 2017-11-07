#include "GameManager/worldmanager.h"

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

WorldManager::WorldManager() : m_modelManager()
{
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f),
		glm::vec3(2.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),
	};

	for (const auto pos : cubePositions) {
		m_objects.emplace_back(
			m_modelManager.getModel("cube.obj"),
			m_modelManager.getTextureId("grassQube.bmp"),
			Transform(pos, glm::vec3()));
	}
}

void WorldManager::onUpdate(IRenderer& renderer, const float deltatime)
{
	for (unsigned int x = 0; x < m_objects.size(); ++x) {
			m_objects[x].onUpdate(renderer, deltatime);
	}
}

