#include "Actor/player.h"

Player::Player() : transform(), m_camera(), m_input() {}

Player::Player(const Transform& transform)
	: transform(transform), m_camera(transform), m_input() {}

Player::~Player() {}

Player & Player::operator=(Player && other) noexcept
{
	transform = std::move(other.transform);
	m_camera.transform = std::move(other.m_camera.transform);
	m_input = std::move(other.m_input);
	return *this;
}

void Player::onUpdate(IRenderer& renderer, float deltatime)
{
	// Process input
	m_input.onUpdate(*this, &renderer, deltatime);
	// Update camera
	m_camera.onUpdate(transform);
	// Render using camera
	renderer.vSetViewMatrix(m_camera.getViewMatrix());
}
