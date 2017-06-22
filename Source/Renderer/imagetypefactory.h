#pragma once

#include <memory>
#include <string>

#include "interfaces.h"

namespace ImageTypeFactory {

std::unique_ptr<IImageType> getImageType(unsigned char const * const data, 
	const std::string& filename);

} //ImageTypeFactory
