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
