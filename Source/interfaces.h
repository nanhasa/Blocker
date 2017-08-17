#pragma once

#include <functional>
#include <memory>
#include <string>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#include <3rdParty/glm/gtc/type_ptr.hpp>
#pragma warning (pop)      // Restore back

class IRenderer {
public:
	virtual ~IRenderer() {};

	virtual bool initialize(std::string&& windowName, int width, int height, 
		std::function<void(float)>&& gameLogic) = 0;
	virtual void startMainLoop() = 0;
	virtual glm::vec3 getCameraFront() = 0;
};

class IImageType {
public:
	virtual ~IImageType() {};

	virtual bool loadFile(std::ifstream& stream) = 0;
	virtual std::unique_ptr<uint8_t[]> decode() = 0;
	virtual int getHeight() const = 0;
	virtual int getWidth() const = 0;
};