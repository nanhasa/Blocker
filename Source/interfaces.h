#pragma once

#include <functional>
#include <memory>
#include <string>

class IRenderer {
public:
	virtual ~IRenderer() {};

	virtual bool initialize(std::string&& windowName, int width, int height, std::function<void()>&& gameLogic) = 0;
	virtual void startMainLoop() = 0;
};

class IImageType {
public:
	virtual ~IImageType() {};

	virtual bool loadFile(std::ifstream& stream) = 0;
	virtual std::unique_ptr<char[]> decode() = 0;
	virtual int getHeight() const = 0;
	virtual int getWidth() const = 0;
};