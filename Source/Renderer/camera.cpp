#include <iostream>

#include "contract.h"
#include "Renderer/camera.h"

Camera::Camera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ) 
	: m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), m_minZoom(2.0f), m_maxZoom(30.0f), 
	 m_mouseSensitivity(0.1f) {

	m_position = glm::vec3(posX, posY, posZ);
	m_cameraFront = glm::normalize(glm::vec3(targetX, targetY, targetZ));
	glm::vec3 directionToCamera = glm::normalize(m_position - m_cameraFront);
	m_cameraRight = glm::normalize(glm::cross(m_worldUp, directionToCamera));
	m_cameraUp = glm::cross(directionToCamera, m_cameraRight);

	m_pitch = 0.0f;
	m_yaw = -90.0f;

	m_dirtyFlag = true;
}

Camera::Camera(glm::vec3 position, glm::vec3 target) 
	: m_position(position), m_cameraFront(glm::normalize(target)), m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	  m_minZoom(2.0f), m_maxZoom(30.0f), m_mouseSensitivity(0.1f) {

	glm::vec3 directionToCamera = glm::normalize(m_position - m_cameraFront);
	m_cameraRight = glm::normalize(glm::cross(m_worldUp, directionToCamera));
	m_cameraUp = glm::cross(directionToCamera, m_cameraRight);

	m_pitch = 0.0f;
	m_yaw = -90.0f;

	m_dirtyFlag = true;
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
	if (m_dirtyFlag) 
		updateVectors();

	return glm::lookAt(m_position, m_position + m_cameraFront, m_worldUp);
}

glm::vec3 Camera::getCameraFront() {
	if (m_dirtyFlag) 
		updateVectors();

	return m_cameraFront;
}

void Camera::updatePosition(glm::vec3 position) {
	if (m_position == position)
		return;

	m_position = position;
	m_dirtyFlag = true;
}

void Camera::processMouseMovement(double xOffset, double yOffset) {
	xOffset *= m_mouseSensitivity;
	yOffset *= m_mouseSensitivity;

	double yaw = m_yaw;
	double pitch = m_pitch;

	m_yaw += xOffset;
	m_pitch += yOffset;

	// Limit pitch value
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	//Check if it is necessary to update dirtyFlag
	if (yaw == m_yaw && pitch == m_pitch)
		return;

	m_dirtyFlag = true;
}

void Camera::processScroll(float scrollOffset) {
	m_zoom -= scrollOffset;
	if (m_zoom > m_maxZoom)
		m_zoom = m_maxZoom;
	if (m_zoom < m_minZoom)
		m_zoom = m_minZoom;
}

void Camera::updateVectors() {
	// Calculate the new m_cameraFront vector
	m_cameraFront.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
	m_cameraFront.y = static_cast<float>(sin(glm::radians(m_pitch)));
	m_cameraFront.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
	m_cameraFront = glm::normalize(m_cameraFront);

	// Also re-calculate the cameraRight and cameraUp vectors
	// Normalize the vectors, because their length gets closer to 0 
	// the more you look up or down which results in slower movement.
	m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_worldUp));  
	m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));

	m_dirtyFlag = false;

	ENSURE(!m_dirtyFlag);
}
