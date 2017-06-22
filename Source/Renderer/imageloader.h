#pragma once

#include <string>
#include <memory>

#include "image.h"
#include "imagetypefactory.h"

class ImageLoader {
public:
	ImageLoader();
	~ImageLoader();

	std::unique_ptr<Image> load(const std::string& file);
};
