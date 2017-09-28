#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

class Transform {
public:

	/**
	 * \brief Transform
	 */
	Transform();

	/**
	 * \brief Transform
	 * \param position
	 * \param rotation
	 */
	Transform(glm::vec3 position, glm::vec3 rotation);

	/**
	 * \brief Transform
	 * \param xPos
	 * \param yPos
	 * \param zPos
	 * \param pitch
	 * \param yaw
	 * \param roll
	 */
	Transform(float xPos, float yPos, float zPos, float pitch, float yaw, float roll);

	/**
	 * \brief ~Transform
	 */
	~Transform();

	glm::vec3 position;
	glm::vec3 rotation;
};
