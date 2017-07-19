#include <fstream>
#include <iostream>

#include "imageloader.h"

namespace texture {

ImageLoader::ImageLoader() {}

ImageLoader::~ImageLoader() {}

std::unique_ptr<Image> ImageLoader::load(const std::string & file) {
	std::ifstream stream(file);
	if (!stream.is_open()) {
		std::cout << "Could not open file: " << file << std::endl;
		return nullptr;
	}


}

} // namespace texture