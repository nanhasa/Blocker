#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

#include "Actor/transform.h"

class Camera {
public:

	/**
	 * \brief Camera
	 * \param transform
	 */
	Camera(Transform transform);

	/**
	 * \brief Desctructor
	 */
	~Camera();

	Transform transform;

	// Prevent copy and assignment
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	/**
	 * \brief Used to access view matrix of this camera. Calculates new view matrix only when m_dirtyFlag is set true
	 * \return View matrix of this camera
	 */
	glm::mat4 getViewMatrix();

	/**
	 * \brief Used to access where camera is pointing at
	 * \return Normalized vector of where camera is pointing at
	 */
	glm::vec3 getCameraFront();

	/**
	 * \brief getCameraRight
	 * \return
	 */
	glm::vec3 getCameraRight();

	/**
	 * \brief getCameraUp
	 * \return
	 */
	glm::vec3 getCameraUp();

	/**
	 * \brief Updates camera's position
	 * \param transf
	 */
	void updateTransform(const Transform& transf);

private:
	glm::vec3 m_cameraUp;		//!< Normalized vector of camera's up direction
	glm::vec3 m_cameraRight;	//!< Normalized vector of camera's right direction
	glm::vec3 m_cameraFront;	//!< Normalized vector of where camera is looking at
	const glm::vec3 m_worldUp;	//!< Normalized constant vector pointing up in world coordinates

	bool m_dirtyFlag; //!< Vectors need updating if true

	/**
	 * \brief Used to update camera vectors. Sets m_dirtyflag to false
	 * \post !m_dirtyFlag
	 */
	void updateVectors();

	/**
	 * \brief calculateCameraFront
	 */
	void calculateCameraFront();
};
