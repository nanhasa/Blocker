#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#include <3rdParty/glm/gtx/quaternion.hpp>
#pragma warning (pop)      // Restore back

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

	glm::dvec3 position;
	glm::dvec3 rotation;
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
	 * \return
	 */
	glm::dmat4 getModelMatrix();

private:
	glm::dvec3 m_up;			//!< Normalized vector of camera's up direction
	glm::dvec3 m_right;			//!< Normalized vector of camera's right direction
	glm::dvec3 m_forward;		//!< Normalized vector of where camera is looking at
	glm::dmat4 m_modelMatrix;	//!< Matrix calculated from quaternion

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
	 * \brief round
	 * \param vector
	 * \param decimals
	 */
	static void round(glm::dvec3& vector, int decimals);

	/**
	 * \brief round
	 * \param mat
	 * \param decimals
	 */
	static void round(glm::dmat4& mat, int decimals);

	/**
	 * \brief fixNegativeZeros
	 * \param vector
	 */
	static void fixNegativeZeros(glm::dvec3& vector);

	/**
	 * \brief fixNegativeZeros
	 * \param mat
	 */
	static void fixNegativeZeros(glm::dmat4& mat);
};
