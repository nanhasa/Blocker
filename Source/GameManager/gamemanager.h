#pragma once

#include <memory>

class Renderer;

class GameManager {
public:

	/**
	 * \brief GameManager
	 */
	GameManager();

	/**
	 * \brief ~GameManager
	 */
	~GameManager();

	/**
	 * \brief start
	 * \return
	 */
	bool start();

	/**
	 * \brief onUpdate
	 */
	void onUpdate();

private:
	std::unique_ptr<Renderer> m_renderer;
};
