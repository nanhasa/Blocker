#pragma once

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

class IRenderer {
public:
	virtual ~IRenderer() {};

	virtual bool vInitialize(std::string&& windowName, std::function<void(float)>&& gameLogic) = 0;
	virtual void vStartMainLoop() = 0;
	virtual void vSetViewMatrix(const glm::mat4& viewMatrix) = 0;
	virtual void vGetCursorPosition(double& x, double& y) const = 0;
	virtual void vCenterCursor() const = 0;
	virtual bool vKeyPressed(int key) const = 0;
	virtual bool vWindowSizeChanged() const = 0;
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

using ListenerId = int;
using EventDataPtr = std::shared_ptr<IEvent>;
using EventDelegate = std::function<void(EventDataPtr)>;
using EventListenerVector = std::vector<std::tuple<ListenerId, std::function<void(EventDataPtr)>>>;

class IEventManager {
public:
	virtual ~IEventManager() {}

	virtual ListenerId registerListener() = 0;
	virtual bool addListener(const EventType evtType, const ListenerId listener, const EventDelegate evtDelegate) = 0;
	virtual bool removeListener(const EventType evtType, const ListenerId listener) = 0;
	virtual void triggerEvent(EventDataPtr pEvent) = 0;
	virtual bool queueEvent(EventDataPtr pEvent) = 0;
	virtual void onUpdate(int msToProcess) = 0;
	virtual unsigned int getQueueLength() = 0;
};

class IConfig {
public:
	virtual ~IConfig() {};

	virtual int get(std::string&& valueName, int defaultValue) = 0;
	virtual float get(std::string&& valueName, float defaultValue) = 0;
	virtual bool get(std::string&& valueName, bool defaultValue) = 0;
	virtual std::string get(std::string&& valueName, std::string defaultValue) = 0;
	virtual glm::vec3 get(std::string&& valueName, glm::vec3 defaultValue) = 0;
};