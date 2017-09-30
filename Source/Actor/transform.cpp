#include "Actor/transform.h"
#include "Utility/contract.h"

const glm::vec3 Transform::worldUp(0.0f, 1.0f, 0.0f);

Transform::Transform() 
	: position(), rotation(), m_up(), m_right(), m_front() {}

Transform::Transform(glm::vec3 position, glm::vec3 rotation) 
	: position(position), rotation(rotation), m_up(), m_right(), m_front() {}

Transform::Transform(float xPos, float yPos, float zPos, float pitch, float yaw, float roll) 
	: position(xPos, yPos, zPos), rotation(pitch, yaw, roll), m_up(), m_right(), m_front() {}

Transform::~Transform() {}

glm::vec3 Transform::getDirectionUp()
{
	if (isCacheDirty())
		updateDirections();
	return m_up;
}

glm::vec3 Transform::getDirectionRight()
{
	if (isCacheDirty())
		updateDirections();
	return m_right;
}

glm::vec3 Transform::getDirectionFront()
{
	if (isCacheDirty())
		updateDirections();
	return m_front;
}

void Transform::updateDirections()
{
	// Calculate front
	m_front.x = static_cast<float>(cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x)));
	m_front.y = static_cast<float>(sin(glm::radians(rotation.x)));
	m_front.z = static_cast<float>(sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x)));
	m_front = glm::normalize(m_front);
	
	// Calculate up and right based on front
	m_right = glm::normalize(glm::cross(m_front, worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));

	// Update cache
	m_cachePos = position;
	m_cacheRot = rotation;
}

bool Transform::isCacheDirty() const
{
	return m_cachePos != position || m_cacheRot != rotation;
}
