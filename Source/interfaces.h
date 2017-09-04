#pragma once

#include <functional>
#include <memory>
#include <string>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

class IRenderer {
public:
	virtual ~IRenderer() {};

	virtual bool vInitialize(std::string&& windowName, int width, int height, 
		std::function<void(float)>&& gameLogic) = 0;
	virtual void vStartMainLoop() = 0;
	virtual glm::vec3 vGetCameraFront() = 0;
};

class IImageType {
public:
	virtual ~IImageType() {};

	virtual bool vLoadFile(std::ifstream& stream) = 0;
	virtual std::unique_ptr<uint8_t[]> vDecode() = 0;
	virtual int vGetHeight() const = 0;
	virtual int vGetWidth() const = 0;
};

using EventType = uint32_t;

class IEvent {
public: 
	virtual ~IEvent() {};

	virtual EventType vGetEventType() const = 0;
	virtual long vGetCreateTime() const = 0;
	virtual std::string vGetEventName() const = 0;
};