#include "Actor/transform.h"

#include "Utility/contract.h"

const glm::dvec3 Transform::worldUp(0, 1, 0);

Transform::Transform() 
	: position(), rotation(), m_up(), m_right(), m_forward(), m_quaternion(), m_modelMatrix() {}

Transform::Transform(glm::dvec3 position, glm::dvec3 rotation) 
	: position(position), rotation(rotation),
	m_up(), m_right(), m_forward(), m_quaternion(), m_modelMatrix() {}

Transform::Transform(double xPos, double yPos, double zPos, double pitch, double yaw, double roll) 
	: position(xPos, yPos, zPos), rotation(pitch, yaw, roll),
	m_up(), m_right(), m_forward(), m_quaternion(), m_modelMatrix() {}

Transform::~Transform() {}

glm::dvec3 Transform::getDirectionUp()
{
	if (glm::length(rotation) == 0)
		return glm::dvec3(0, 1, 0);
	if (isCacheDirty())
		updateDirections();
	return m_up;
}

glm::dvec3 Transform::getDirectionRight()
{
	if (glm::length(rotation) == 0)
		return glm::dvec3(1, 0, 0);
	if (isCacheDirty())
		updateDirections();
	return m_right;
}

glm::dvec3 Transform::getDirectionForward()
{
	if (glm::length(rotation) == 0)
		return glm::dvec3(0, 0, -1);
	if (isCacheDirty())
		updateDirections();
	return m_forward;
}

glm::dmat4 Transform::getModelMatrix()
{
	if (isCacheDirty())
		updateDirections();
	return m_modelMatrix;
}

void Transform::updateDirections()
{
	// Calculate quaternion axis angles
	auto x = glm::angleAxis(glm::radians(rotation.x), glm::dvec3(1, 0, 0));
	auto y = glm::angleAxis(glm::radians(rotation.y), glm::dvec3(0, 1, 0));
	auto z = glm::angleAxis(glm::radians(rotation.z), glm::dvec3(0, 0, 1));

	// Build new quaternion
	m_quaternion = glm::normalize(y * x * z);

	// Update direction vectors
	m_up = m_quaternion * glm::dvec3(0, 1, 0);
	m_right = m_quaternion * glm::dvec3(1, 0, 0);
	m_forward = m_quaternion * glm::dvec3(0, 0, -1);

	// Round decimals to 5 decimals to clean inaccuracies in degree -> radians conversion
	round(m_up, 5);
	round(m_right, 5);
	round(m_forward, 5);

	// Something beyond me in glm creates -0.00000 numbers, let's update those to good ol' regular zeros
	fixNegativeZeros(m_up);
	fixNegativeZeros(m_right);
	fixNegativeZeros(m_forward);

	// Calculate model matrix
	m_modelMatrix = glm::toMat4(m_quaternion);
	round(m_modelMatrix, 5);
	fixNegativeZeros(m_modelMatrix);

	// Update cache
	m_cacheRotation = rotation;
}

bool Transform::isCacheDirty() const
{
	return m_cacheRotation != rotation;
}

void Transform::round(glm::dvec3& vector, int decimals)
{
	vector.x = glm::round(vector.x * glm::pow(10, decimals)) / glm::pow(10, decimals);
	vector.y = glm::round(vector.y * glm::pow(10, decimals)) / glm::pow(10, decimals);
	vector.z = glm::round(vector.z * glm::pow(10, decimals)) / glm::pow(10, decimals);
}

void Transform::round(glm::dmat4 & mat, int decimals)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			mat[i][j] = glm::round(mat[i][j] * glm::pow(10, decimals)) / glm::pow(10, decimals);
		}
	}
}

void Transform::fixNegativeZeros(glm::dvec3& vector)
{
	// For some reason glm::cross may create -0.000000 values
	// Fix them by assigning 0 on them
	if (vector.x == 0) vector.x = 0;
	if (vector.y == 0) vector.y = 0;
	if (vector.z == 0) vector.z = 0;
}

void Transform::fixNegativeZeros(glm::dmat4& mat)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (mat[i][j] == 0)
				mat[i][j] = 0;
		}
	}
}
