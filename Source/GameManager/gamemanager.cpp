#include <iostream>

#include "gamemanager.h"
#include "Renderer\renderer.h"
#include "contract.h"

GameManager::GameManager() {}

GameManager::~GameManager() {} 

bool GameManager::start() { 
	m_renderer = std::make_unique<Renderer>();
	if (m_renderer->initialize("Blocker", 800, 600, std::bind(&GameManager::onUpdate, this))) {
		m_renderer->startMainLoop(); //Start main loop
		return true;
	}
	return false;
}

void GameManager::onUpdate() {
	//std::cout << "gamemanager onUpdate called" << std::endl;
}
