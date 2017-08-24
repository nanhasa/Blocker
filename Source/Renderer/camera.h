#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#include <3rdParty/glm/gtc/type_ptr.hpp>
#pragma warning (pop)      // Restore back


class Camera {
public:

	/**
	 * \brief Builder. Creates valid camera object
	 * \param posX Start position on x-axis
	 * \param posY Start position on y-axis
	 * \param posZ Start position on z-axis
	 * \param targetX Start position where camera looks at on x-axis
	 * \param targetY Start position where camera looks at on y-axis
	 * \param targetZ Start position where camera looks at on z-axis
	 */
	Camera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);

	/**
	 * \brief Builder. Creates valid camera object
	 * \param position Start position
	 * \param target Start position where camera looks at
	 */
	Camera(glm::vec3 position, glm::vec3 target);

	/**
	 * \brief Desctructor
	 */
	~Camera();

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
	 * \brief Updates camera's position
	 * \param position
	 */
	void updatePosition(glm::vec3 position);

	/**
	 * \brief Updates m_pitch and m_yaw based on mouse movement offset
	 * \param xOffset The amount mouse moved on x-axis since last update
	 * \param yOffset The amount mouse moved on y-axis since last update
	 */
	void processMouseMovement(double xOffset, double yOffset);

	/**
	 * \brief Used to zoom in and out
	 * \param scrollOffset
	 */
	void processScroll(float scrollOffset);

private:
	glm::vec3 m_position;		//!< Camera position
	glm::vec3 m_cameraUp;		//!< Normalized vector of camera's up direction
	glm::vec3 m_cameraRight;	//!< Normalized vector of camera's right direction
	glm::vec3 m_cameraFront;	//!< Normalized vector of where camera is looking at
	const glm::vec3 m_worldUp;	//!< Normalized constant vector pointing up in world coordinates

	double m_pitch;	//!< Current rotation on x-axis
	double m_yaw;	//!< Current rotation on y-axis
	float m_zoom;	//!< Current zoom value

	bool m_dirtyFlag; //!< Vectors need updating if true

	const float m_minZoom;			//!< Limit on minimum zoom
	const float m_maxZoom;			//!< Limit on maximum zoom
	const float m_mouseSensitivity;	//!< Multiplier used to make camera movement more or less sensitive

	/**
	 * \brief Used to update camera vectors. Sets m_dirtyflag to false
	 * \post !m_dirtyFlag
	 */
	void updateVectors();
};
