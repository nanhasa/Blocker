#pragma once

#include <string>
#include <functional>
#include <memory>

class IRenderer {
public:
	virtual ~IRenderer() {};

	virtual bool initialize(std::string&& windowName, int width, int height, std::function<void()>&& gameLogic) = 0;
	virtual void startMainLoop() = 0;
};

class IImageType {
	virtual ~IImageType() {};

	virtual std::unique_ptr<unsigned char[]> decode(std::unique_ptr<unsigned char[]> rawdata,
		int* width, int* height) = 0;
};