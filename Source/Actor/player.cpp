#include "Actor/player.h"

#include "Utility/utility.h"

#include <iostream>

Player::Player(const Transform& transform) 
	: transform(transform), m_camera(transform), m_input() {}

Player::~Player() {}

void Player::onUpdate(IRenderer& renderer, float deltatime)
{
	// Process input
	m_input.onUpdate(*this, renderer, m_camera, deltatime);
	// Update camera
	m_camera.updateTransform(transform);
	// Render using camera
	renderer.vSetViewMatrix(m_camera.getViewMatrix());
}
