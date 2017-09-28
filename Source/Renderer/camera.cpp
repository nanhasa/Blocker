#include "Renderer/camera.h"

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#pragma warning (pop)      // Restore back

#include "Utility/contract.h"

Camera::Camera(Transform transform)
	: transform(transform), m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f))
{
	calculateCameraFront();
	const glm::vec3 directionToCamera = glm::normalize(transform.position - m_cameraFront);
	m_cameraRight = glm::normalize(glm::cross(m_worldUp, directionToCamera));
	m_cameraUp = glm::cross(directionToCamera, m_cameraRight);

	m_dirtyFlag = true;
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix()
{
	if (m_dirtyFlag)
		updateVectors();

	return glm::lookAt(transform.position, transform.position + m_cameraFront, m_worldUp);
}

glm::vec3 Camera::getCameraFront()
{
	if (m_dirtyFlag)
		updateVectors();

	return m_cameraFront;
}

glm::vec3 Camera::getCameraRight()
{
	if (m_dirtyFlag)
		updateVectors();

	return m_cameraRight;
}

glm::vec3 Camera::getCameraUp()
{
	if (m_dirtyFlag)
		updateVectors();

	return m_cameraUp;
}

void Camera::updateTransform(const Transform& transf)
{
	if (transform.position == transf.position && transform.rotation == transf.rotation)
		return;

	transform = transf;
	m_dirtyFlag = true;
}

void Camera::updateVectors()
{
	// Calculate new camera front direction
	calculateCameraFront();

	// Also re-calculate the cameraRight and cameraUp vectors
	// Normalize the vectors, because their length gets closer to 0 
	// the more you look up or down which results in slower movement.
	m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_worldUp));
	m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));

	m_dirtyFlag = false;

	ENSURE(!m_dirtyFlag);
}

void Camera::calculateCameraFront()
{
	m_cameraFront.x = static_cast<float>(cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x)));
	m_cameraFront.y = static_cast<float>(sin(glm::radians(transform.rotation.x)));
	m_cameraFront.z = static_cast<float>(sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x)));
	m_cameraFront = glm::normalize(m_cameraFront);

	m_dirtyFlag = true;
}
