#include "imagetypefactory.h"

namespace texture {
namespace imageTypeFactory {

std::unique_ptr<IImageType> getImageType(unsigned char const * const data, const std::string & filename)
{
	return nullptr; //TODO: implement logic which image type is loaded
}

} // namespace imageTypeFactory
} // namespace texture