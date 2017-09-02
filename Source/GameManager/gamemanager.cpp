#include <functional>
#include <iostream>

#pragma warning (push, 2)  // Temporarily set warning level 2
#pragma warning (pop)      // Restore back

#include "GameManager/gamemanager.h"
#include "Renderer/renderer.h"

GameManager::GameManager() {}

GameManager::~GameManager() {}

bool GameManager::start()
{
	m_renderer = std::make_unique<Renderer>();
	if (m_renderer->vInitialize("Blocker", 800, 600,
	                            std::bind(&GameManager::onUpdate, this, std::placeholders::_1))) {
		//EventManager::addListener(InputCommandEvent::eventType,
		//	std::bind(&GameManager::testDelegate, this, std::placeholders::_1));

		//EventManager::removeListener(InputCommandEvent::eventType,
		//	std::bind(&GameManager::testDelegate, this, std::placeholders::_1));

		m_renderer->vStartMainLoop(); //Start main loop
		return true;
	}
	return false;
}

void GameManager::onUpdate(float deltatime)
{
	(void)deltatime;
	//auto camera = m_renderer->vGetCameraFront();
	//std::cout << "delta: " << deltatime << " Camera front: " << camera.x << "f " << camera.y << "f " << camera.z << "f" << std::endl;
}

void GameManager::testDelegate(std::shared_ptr<IEvent> eventData)
{
	std::cout << "testDelegate GameManager" << std::endl;
}
