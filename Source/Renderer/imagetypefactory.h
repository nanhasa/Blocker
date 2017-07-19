#pragma once

#include <memory>
#include <string>

#include "interfaces.h"

namespace texture {
namespace imageTypeFactory {

std::unique_ptr<IImageType> getImageType(unsigned char const * const data,
	const std::string& filename);

} // namespace imageTypeFactory
} // namespace texture
