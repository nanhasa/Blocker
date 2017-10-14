#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

#include "Actor/transform.h"

class Camera {
public:

	/**
	 * \brief Constructor to set camera start at 0, 0, 0 position and rotation
	 */
	Camera();

	/**
	 * \brief Constructor to set camera start at certain position and rotation
	 * \param transform Transform where camera starts
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
	Camera& operator=(Camera&& rhs) = delete;

	/**
	 * \brief Used to access view matrix of this camera
	 * \return View matrix of this camera
	 */
	glm::mat4 getViewMatrix();

	/**
	 * \brief Updates the transformation of camera
	 * \param transf Value used to update
	 */
	void onUpdate(const Transform& transf);

};
