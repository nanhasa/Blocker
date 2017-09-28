#include "GameManager/gamemanager.h"

#include <functional>

#include "Renderer/renderer.h"

GameManager::GameManager(): m_player(Transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)), m_log("GameManager") {}

GameManager::~GameManager() {}

bool GameManager::start()
{
	m_renderer = std::make_unique<Renderer>();
	if (m_renderer->vInitialize("Blocker", 800, 600,
	                            std::bind(&GameManager::onUpdate, this, std::placeholders::_1))) {

		m_renderer->vStartMainLoop(); //Start main loop
		return true;
	}
	return false;
}

void GameManager::onUpdate(float deltatime)
{
	//m_player.processInput(m_renderer->vGetMouseOffset(), m_renderer->vGetKeyInput());
	m_player.onUpdate(*m_renderer.get(), deltatime);
}
