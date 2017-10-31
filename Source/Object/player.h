#pragma once

#include "interfaces.h"
#include "Object/inputmanager.h"
#include "Object/camera.h"
#include "Object/object.h"
#include "Object/transform.h"

class Player : public Object{
public:

	/**
	 * \brief Constructor that initializes Player to 0, 0, 0 position and rotation
	 */
	Player();

	/**
	 * \brief Constructor that initializes object to certain position and rotation
	 * \param transform Used to set the position and rotation
	 */
	explicit Player(const Transform& transform);

	~Player() = default;

	Player(Player const&) = delete;
	Player& operator=(const Player& rhs) = delete;
	Player& operator=(Player&& rhs) noexcept;

	/**
	 * \brief Destructor
	 * \param renderer Used to render object after update
	 * \param deltatime Time in seconds since last frame
	 */
	void onUpdate(IRenderer& renderer, const float deltatime) override;

private:
	Camera m_camera;		//!< First person camera
	InputManager m_input;	//!< Used to manage mouse and key input
};
