#include "Actor/transform.h"

Transform::Transform() : position(glm::vec3(0.0f, 0.0f, 0.0f)), rotation(glm::vec3(0.0f, 0.0f, 0.0f)) {}

Transform::Transform(glm::vec3 position, glm::vec3 rotation) : position(position), rotation(rotation) {}

Transform::Transform(float xPos, float yPos, float zPos, float pitch, float yaw, float roll) 
	: position(glm::vec3(xPos, yPos, zPos)), rotation(glm::vec3(pitch, yaw, roll)) {}

Transform::~Transform() {}
