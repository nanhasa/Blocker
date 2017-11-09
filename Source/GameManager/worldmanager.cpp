#include "GameManager/worldmanager.h"

#include "GameManager/terrainfactory.h"

WorldManager::WorldManager() : m_modelManager()
{
	terrainFactory::initializeWorld(m_objects, 8, m_modelManager);
}

void WorldManager::onUpdate(IRenderer& renderer, const float deltatime)
{
	for (unsigned int x = 0; x < m_objects.size(); ++x) {
			m_objects[x].onUpdate(renderer, deltatime);
	}
}

