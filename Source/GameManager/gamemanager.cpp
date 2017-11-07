#include "GameManager/gamemanager.h"

#include <functional>

#include "Renderer/renderer.h"
#include "Utility/config.h"
#include "Utility/locator.h"

GameManager::GameManager() 
	: m_renderer(nullptr), m_player(), m_log("GameManager"), m_world(nullptr) {}

bool GameManager::start()
{
	m_renderer = std::make_unique<Renderer>();
	if (m_renderer->vInitialize("Blocker", std::bind(&GameManager::onUpdate, this, std::placeholders::_1))) {

		m_player = Player(
			Transform(Locator::getConfig()->get("PlayerStartPosition", glm::vec3(0, 0, 7)),
				Locator::getConfig()->get("PlayerStartRotation", glm::vec3()))
		);

		m_world = std::make_unique<WorldManager>();

		m_renderer->vStartMainLoop(); //Start main loop
		return true;
	}
	m_log.fatal("Renderer was not initialized properly");
	return false;
}

void GameManager::onUpdate(const float deltatime)
{
	m_player.onUpdate(*m_renderer.get(), deltatime);
	m_world->onUpdate(*m_renderer.get(), deltatime);
}
