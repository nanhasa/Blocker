#include "GameManager/worldmanager.h"

#include "GameManager/terrainfactory.h"

WorldManager::WorldManager() : m_modelManager()
{
	terrainFactory::initializeWorld(m_objects, 20000, m_modelManager);
}

void WorldManager::onUpdate(Player& player, IRenderer& renderer, const float deltatime)
{
	(void)player;
	for (unsigned int x = 0; x < m_objects.size(); ++x) {
		// tee joukko virtual array kokonaisuus eli joukko pienempiä arrayta joita vaihdetaan lennosta
		m_objects[x]->onUpdate(renderer, deltatime);
	}
}

