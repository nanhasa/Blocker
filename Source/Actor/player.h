#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "Actor/inputmanager.h"
#include "Actor/transform.h"
#include "interfaces.h"
#include "Renderer/camera.h"

class Player {
public:

	/**
	 * \brief Player
	 * \param transform
	 * \param screenResX
	 * \param screenResY
	 */
	Player(const Transform& transform, int screenResX, int screenResY);

	/**
	 * \brief ~Player
	 */
	~Player();

	Transform transform;

	/**
	 * \brief onUpdate
	 * \param renderer
	 * \param deltatime
	 */
	void onUpdate(IRenderer& renderer, float deltatime);

private:
	Camera m_camera;
	InputManager m_input;
};
