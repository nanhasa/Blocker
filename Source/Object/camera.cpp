#include "Object/camera.h"

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#pragma warning (pop)      // Restore back

Camera::Camera() : transform() {}

Camera::Camera(Transform transform) : transform(transform){}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(transform.position, transform.position + transform.getDirectionForward(), transform.worldUp);
}

void Camera::onUpdate(const Transform& transf)
{
	transform = transf;
}