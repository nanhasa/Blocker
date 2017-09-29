#include "Actor/player.h"

#include "Utility/utility.h"

#include <iostream>

Player::Player(const Transform& transform, int screenResX, int screenResY) 
	: transform(transform), m_camera(transform), m_input(screenResX, screenResY) {}

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
