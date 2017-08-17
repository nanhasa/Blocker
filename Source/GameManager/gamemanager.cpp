#include <iostream>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#include <3rdParty/glm/gtc/type_ptr.hpp>
#pragma warning (pop)      // Restore back

#include "GameManager/gamemanager.h"
#include "Renderer/renderer.h"
#include "contract.h"

GameManager::GameManager() {}

GameManager::~GameManager() {} 

bool GameManager::start() { 
	m_renderer = std::make_unique<Renderer>();
	if (m_renderer->initialize("Blocker", 800, 600, 
		std::bind(&GameManager::onUpdate, this, std::placeholders::_1))) {
		m_renderer->startMainLoop(); //Start main loop
		return true;
	}
	return false;
}

void GameManager::onUpdate(float deltatime) {
	auto camera = m_renderer->getCameraFront();
	std::cout << "delta: " << deltatime << " Camera front: " << camera.x << "f " << camera.y << "f " << camera.z << "f" << std::endl;
}

void GameManager::deviceInput(int keyvalue) {
	(void)keyvalue;
}
