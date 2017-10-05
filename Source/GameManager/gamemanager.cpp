#include "GameManager/gamemanager.h"

#include <functional>

#include "Renderer/renderer.h"

GameManager::GameManager(): m_player(Transform(0, 0, 7, 0, 0, 0)), m_log("GameManager") {}

GameManager::~GameManager() {}

bool GameManager::start()
{
	m_renderer = std::make_unique<Renderer>();
	if (m_renderer->vInitialize("Blocker", 1600, 1200,
	                            std::bind(&GameManager::onUpdate, this, std::placeholders::_1))) {
		m_renderer->vStartMainLoop(); //Start main loop
		return true;
	}
	m_log.fatal("Renderer was not initialized properly");
	return false;
}

void GameManager::onUpdate(float deltatime)
{
	m_player.onUpdate(*m_renderer.get(), deltatime);
}
