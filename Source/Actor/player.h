#pragma once

#include "Actor/inputmanager.h"
#include "Actor/transform.h"
#include "interfaces.h"
#include "Actor/camera.h"

class Player {
public:

	/**
	 * \brief Constructor that initializes Player to 0, 0, 0 position and rotation
	 */
	Player();

	/**
	 * \brief Constructor that initializes object to certain position and rotation
	 * \param transform Used to set the position and rotation
	 */
	Player(const Transform& transform);

	/**
	 * \brief Destructor
	 */
	~Player();

	Player(Player const&) = delete;
	Player& operator=(const Player& rhs) = delete;
	Player& operator=(Player&& rhs) noexcept;

	Transform transform;

	/**
	 * \brief Called on every frame to update the object
	 * \param renderer Used to render object after update
	 * \param deltatime Time in seconds since last frame
	 */
	void onUpdate(IRenderer& renderer, float deltatime);

private:
	Camera m_camera;		//!< First person camera
	InputManager m_input;	//!< Used to manage mouse and key input
};
