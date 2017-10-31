#include "GameManager/worldmanager.h"

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

WorldManager::WorldManager(ModelManager& modelManager)
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
	//	//glm::vec3(2.4f, -0.4f, -3.5f),
	//	//glm::vec3(-1.7f, 3.0f, -7.5f),
	//	//glm::vec3(1.3f, -2.0f, -2.5f),
	//	//glm::vec3(1.5f, 2.0f, -2.5f),
	//	//glm::vec3(1.5f, 0.2f, -1.5f),
	//	//glm::vec3(-1.3f, 1.0f, -1.5f)
	};
	
	for (const auto pos : cubePositions) {
		m_objects.emplace_back(Terrain(modelManager, Transform(pos, glm::vec3())));
	}
}

void WorldManager::onUpdate(IRenderer& renderer, const float deltatime)
{
	//for (unsigned int x = 0; x < m_objects.size(); ++x) {
	//	for (unsigned int z = 0; z < m_objects[x].size(); ++z)
	//		m_objects[x][z].onUpdate(renderer, deltatime);
	//
	for (unsigned int x = 0; x < m_objects.size(); ++x) {
			m_objects[x].onUpdate(renderer, deltatime);
	}
}

