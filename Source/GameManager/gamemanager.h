#pragma once

#include <memory>

class Renderer;

class GameManager {
public:
	GameManager();
	~GameManager();

	bool start();
	void onUpdate();

private:
	std::unique_ptr<Renderer> m_renderer;
};