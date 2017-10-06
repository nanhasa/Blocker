#include "Actor/transform.h"

#include "Utility/contract.h"

const glm::vec3 Transform::worldUp(0, 1, 0);

Transform::Transform() 
	: position(), rotation(), m_up(), m_right(), m_forward(), m_quaternion(), m_rotationMatrix() {}

Transform::Transform(glm::vec3 position, glm::vec3 rotation) 
	: position(position), rotation(rotation),
	m_up(), m_right(), m_forward(), m_quaternion(), m_rotationMatrix() {}

Transform::Transform(float xPos, float yPos, float zPos, float pitch, float yaw, float roll) 
	: position(xPos, yPos, zPos), rotation(pitch, yaw, roll),
	m_up(), m_right(), m_forward(), m_quaternion(), m_rotationMatrix() {}

Transform::~Transform() {}

glm::vec3 Transform::getDirectionUp()
{
	if (glm::length(rotation) == 0)
		return glm::vec3(0, 1, 0);
	if (isCacheDirty())
		updateDirections();
	return m_up;
}

glm::vec3 Transform::getDirectionRight()
{
	if (glm::length(rotation) == 0)
		return glm::vec3(1, 0, 0);
	if (isCacheDirty())
		updateDirections();
	return m_right;
}

glm::vec3 Transform::getDirectionForward()
{
	if (glm::length(rotation) == 0)
		return glm::vec3(0, 0, -1);
	if (isCacheDirty())
		updateDirections();
	return m_forward;
}

glm::mat4 Transform::getRotationMatrix()
{
	if (isCacheDirty())
		updateDirections();
	return m_rotationMatrix;
}

void Transform::updateDirections()
{
	clampRotations();

	// Calculate quaternion axis angles
	auto xAngle = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1, 0, 0));
	auto yAngle = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0, 1, 0));
	auto zAngle = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0, 0, 1));

	// Build new quaternion
	m_quaternion = glm::normalize(yAngle * xAngle * zAngle);

	// Update direction vectors
	m_up = m_quaternion * glm::vec3(0, 1, 0);
	m_right = m_quaternion * glm::vec3(1, 0, 0);
	m_forward = m_quaternion * glm::vec3(0, 0, -1);

	// Round decimals to 5 decimals to clean inaccuracies in degree -> radians conversion
	round(m_up, 5);
	round(m_right, 5);
	round(m_forward, 5);

	// Something beyond me in glm creates -0.00000 numbers, let's update those to good ol' regular zeros
	fixNegativeZeros(m_up);
	fixNegativeZeros(m_right);
	fixNegativeZeros(m_forward);

	// Calculate model matrix
	m_rotationMatrix = glm::toMat4(m_quaternion);
	round(m_rotationMatrix, 5);
	fixNegativeZeros(m_rotationMatrix);

	// Update cache
	m_cacheRotation = rotation;
}

bool Transform::isCacheDirty() const
{
	return m_cacheRotation != rotation;
}

void Transform::round(glm::vec3& vector, int decimals)
{
	vector.x = static_cast<float>(glm::round(vector.x * glm::pow(10, decimals)) / glm::pow(10, decimals));
	vector.y = static_cast<float>(glm::round(vector.y * glm::pow(10, decimals)) / glm::pow(10, decimals));
	vector.z = static_cast<float>(glm::round(vector.z * glm::pow(10, decimals)) / glm::pow(10, decimals));
}

void Transform::round(glm::mat4 & mat, int decimals)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			mat[i][j] = static_cast<float>(glm::round(mat[i][j] * glm::pow(10, decimals)) / glm::pow(10, decimals));
		}
	}
}

void Transform::fixNegativeZeros(glm::vec3& vector)
{
	// For some reason glm::cross may create -0.000000 values
	// Fix them by assigning 0 on them
	if (vector.x == 0) vector.x = 0;
	if (vector.y == 0) vector.y = 0;
	if (vector.z == 0) vector.z = 0;
}

void Transform::fixNegativeZeros(glm::mat4& mat)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (mat[i][j] == 0)
				mat[i][j] = 0;
		}
	}
}

void Transform::clampRotations()
{
	// Remove full circles
	if (glm::abs(rotation.y) > 360)
		rotation.y = glm::mod(rotation.y, 360.0f);
	if (glm::abs(rotation.x) > 360)
		rotation.x = glm::mod(rotation.x, 360.0f);
	if (glm::abs(rotation.z) > 360)
		rotation.z = glm::mod(rotation.z, 360.0f);

	// Turn negative angles to positive
	if (rotation.y < 0)
		rotation.y += 360;
	if (rotation.x < 0)
		rotation.x += 360;
	if (rotation.z < 0)
		rotation.z += 360;
}
