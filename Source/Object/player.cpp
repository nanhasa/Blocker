#include "Object/player.h"

Player::Player() : Object(), m_camera(), m_input() {}

Player::Player(const Transform& transform)
	: Object(transform), m_camera(transform), m_input() {}

Player & Player::operator=(Player && other) noexcept
{
	transform = std::move(other.transform);
	m_camera.transform = std::move(other.m_camera.transform);
	m_input = std::move(other.m_input);
	return *this;
}

void Player::onUpdate(IRenderer& renderer, const float deltatime)
{
	// Process input
	m_input.onUpdate(*this, &renderer, deltatime);
	// Update camera
	m_camera.onUpdate(transform);
	// Set the view matrix
	renderer.vGetShaderProgram()->setMat4("view", m_camera.getViewMatrix());
}
