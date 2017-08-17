#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#include <3rdParty/glm/gtc/type_ptr.hpp>
#pragma warning (pop)      // Restore back


class Camera {
public:
	Camera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
	Camera(glm::vec3 position, glm::vec3 target);
	
	~Camera();

	// Prevent copy and assignment
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	glm::mat4 getViewMatrix();
	glm::vec3 getCameraFront();

	void updatePosition(glm::vec3 position);
	void processMouseMovement(double xOffset, double yOffset);
	void processScroll(float scrollOffset);

private:
	glm::vec3 m_position;
	glm::vec3 m_cameraUp;
	glm::vec3 m_cameraRight;
	glm::vec3 m_cameraFront;
	const glm::vec3 m_worldUp;

	double m_pitch;	// rotation on x
	double m_yaw;	// rotation on y
	float m_zoom;

	bool m_dirtyFlag; // Vectors need updating if true

	const float m_minZoom;
	const float m_maxZoom;
	const float m_mouseSensitivity;

	void updateVectors();
};