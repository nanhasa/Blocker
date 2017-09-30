#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
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

	glm::vec3 position;
	glm::vec3 rotation;
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
	glm::vec3 getDirectionFront();

private:
	glm::vec3 m_up;		//!< Normalized vector of camera's up direction
	glm::vec3 m_right;	//!< Normalized vector of camera's right direction
	glm::vec3 m_front;	//!< Normalized vector of where camera is looking at

	glm::vec3 m_cachePos;	//!< Holds position when directions were last calculated
	glm::vec3 m_cacheRot;	//!< Holds rotation when directions were last calculated

	/**
	 * \brief Used to update direction and cache vectors
	 */
	void updateDirections();

	/**
	 * \brief Used to know if direction vectors need updating
	 * \return true if either cache vector is different from the public ones
	 */
	bool isCacheDirty() const;
};
