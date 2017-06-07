#pragma once

#include <string>
#include <functional>

class IRenderer {
public:
	virtual ~IRenderer() {};

	virtual bool init(std::string&& windowName, int width, int height, std::function<void()>&& gameLogic) = 0;
	virtual void render() = 0;
};