#include "Actor/player.h"

Player::Player(const Transform& transform) 
	: transform(transform), m_camera(transform), m_input() {}

Player::~Player() {}

void Player::onUpdate(IRenderer& renderer, float deltatime)
{
	// Process input
	m_input.onUpdate(*this, &renderer, deltatime);
	// Update camera
	m_camera.onUpdate(transform);
	// Render using camera
	renderer.vSetViewMatrix(m_camera.getViewMatrix());
}
