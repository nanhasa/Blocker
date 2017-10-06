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
	Transform(glm::dvec3 position, glm::dvec3 rotation);

	/**
	 * \brief Constructor
	 * \param xPos Position on x axis
	 * \param yPos Position on y axis
	 * \param zPos Position on z axis
	 * \param pitch Rotation on x axis
	 * \param yaw Rotation on y axis
	 * \param roll Rotation on z axis
	 */
	Transform(double xPos, double yPos, double zPos, double pitch, double yaw, double roll);

	/**
	 * \brief Desctructor
	 */
	~Transform();

	glm::dvec3 position;				//!< Object's world position (x, y, z)
	glm::dvec3 rotation;				//!< Object's euler angle rotation (pitch, yaw, roll)
	static const glm::dvec3 worldUp;	//!< Normalized vector pointing up in world coordinates

	/**
	 * \brief Used to get normalized vector pointing upwards from current rotation
	 * \return Normalized vector pointing up
	 */
	glm::dvec3 getDirectionUp();

	/**
	 * \brief Used to get normalized vector pointing right from current rotation
	 * \return Normalized vector pointing right
	 */
	glm::dvec3 getDirectionRight();

	/**
	 * \brief Used to get normalized vector pointing forward from current rotation
	 * \return Normalized vector pointing forward
	 */
	glm::dvec3 getDirectionForward();

	/**
	 * \brief getModelMatrix
	 * \return Matrix representing object rotation (position not included at the moment)
	 */
	glm::dmat4 getModelMatrix();

private:
	glm::dvec3 m_up;			//!< Normalized vector of camera's up direction
	glm::dvec3 m_right;			//!< Normalized vector of camera's right direction
	glm::dvec3 m_forward;		//!< Normalized vector of where camera is looking at
	glm::dmat4 m_modelMatrix;	//!< Matrix calculated from quaternion, at the moment only holds rotation (position should probably be also calculated in it)

	glm::dquat m_quaternion;	//!< Rotation is managed in the back-end with a quaternion while the public one is in euler's angle

	glm::dvec3 m_cacheRotation;	//!< Holds rotation when directions were last calculated

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
	* \brief Used to round all values in vector to given decimals accuracy
	* \param vector Vector to be updated
	* \param decimals number of decimals to be saved
	*/
	static void round(glm::dvec3& vector, int decimals);

	/**
	 * \brief Used to round all values in matrice to given decimals accuracy
	 * \param mat Matrice to be updated
	 * \param decimals number of decimals to be saved
	 */
	static void round(glm::dmat4& mat, int decimals);

	/**
	 * \brief Used to fix this weird glm thing where there are -0.00000 values
	 * \param vector Vector to be updated
	 */
	static void fixNegativeZeros(glm::dvec3& vector);

	/**
	 * \brief Used to fix this weird glm thing where there are -0.00000 values
	 * \param mat Matrice to be updated
	 */
	static void fixNegativeZeros(glm::dmat4& mat);

	/**
	 * \brief Used to update rotations to between 0..360
	 */
	void clampRotations();
};
