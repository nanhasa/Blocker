#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#include <3rdParty/glm/gtx/quaternion.hpp>
#pragma warning (pop)      // Restore back


// Class to calculate object position and rotation in 3D world
// The rotations work in left-handed coordinate (positie x goes right, positive y goes up, positive z goes towards camera)
// Increasing pitch rotation value rotates object left around y axis
// Increasing yaw rotation value rotates object up around x axis
// Increasing roll rotation value rolls object left around z axis
// Rotation values are clamped between 0..360, which is calculated whenever lazy calculation happens
//
// Rotations are publicly shown as euler angles, but under the hood it is stored as quaternion
// Direction vectors and matrix are lazy calculated when called if rotation cache is not up to date
class Transform {
public:

	/**
	 * \brief Constructor that sets initial position and rotation to 0, 0, 0
	 */
	Transform();

	/**
	 * \brief Constructor
	 * \param position Initial position
	 * \param rotation Initial rotation
	 */
	Transform(glm::vec3 position, glm::vec3 rotation);

	/**
	 * \brief Constructor
	 * \param xPos Position on x axis
	 * \param yPos Position on y axis
	 * \param zPos Position on z axis
	 * \param pitch Rotation on x axis
	 * \param yaw Rotation on y axis
	 * \param roll Rotation on z axis
	 */
	Transform(float xPos, float yPos, float zPos, float pitch, float yaw, float roll);

	/**
	 * \brief Desctructor
	 */
	~Transform();

	glm::vec3 position;				//!< Object's world position (x, y, z)
	glm::vec3 rotation;				//!< Object's euler angle rotation (pitch, yaw, roll)
	static const glm::vec3 worldUp;	//!< Normalized vector pointing up in world coordinates

	/**
	 * \brief Used to get normalized vector pointing upwards from current rotation
	 * \return Normalized vector pointing up
	 */
	glm::vec3 getDirectionUp();

	/**
	 * \brief Used to get normalized vector pointing right from current rotation
	 * \return Normalized vector pointing right
	 */
	glm::vec3 getDirectionRight();

	/**
	 * \brief Used to get normalized vector pointing forward from current rotation
	 * \return Normalized vector pointing forward
	 */
	glm::vec3 getDirectionForward();

	/**
	 * \brief getModelMatrix
	 * \return Matrix representing object rotation
	 */
	glm::mat4 getRotationMatrix();

private:
	glm::vec3 m_up;				//!< Normalized vector of camera's up direction
	glm::vec3 m_right;			//!< Normalized vector of camera's right direction
	glm::vec3 m_forward;		//!< Normalized vector of where camera is looking at
	glm::mat4 m_rotationMatrix;	//!< Matrix calculated from quaternion, at the moment only holds rotation
	glm::quat m_quaternion;		//!< Rotation is managed in the back-end with a quaternion while the public one is in euler's angle

	glm::vec3 m_cacheRotation;	//!< Holds rotation when directions were last calculated

	/**
	 * \brief Used to update direction and cache vectors
	 */
	void updateDirections();

	/**
	 * \brief Used to know if direction vectors need updating
	 * \return true if either cache vector is different from the public ones
	 */
	bool isCacheDirty() const;

	/**
	 * \brief Used to update rotations to between 0..360
	 */
	void clampRotations();
};
