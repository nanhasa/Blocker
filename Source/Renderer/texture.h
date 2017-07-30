#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "interfaces.h"

namespace texture {

	class Image {
	public:
		Image(std::unique_ptr<char[]> data, int width, int height);
		~Image();

		int getWidth() const;
		int getHeight() const;
		char* getData() const;

		void flipVertically();
		void flipHorizontally();

	private:
		std::unique_ptr<char[]> m_data;
		const int m_width;
		const int m_height;
	};

	std::unique_ptr<Image> load(const std::string& file);

	bool isStreamEmpty(std::ifstream& stream);

	bool validateFile(std::ifstream& stream);

	std::streampos getFileSize(std::ifstream& stream);

namespace factory {

	std::unique_ptr<IImageType> getImageType(const std::string& filename);

} // namespace factory
} // namespace texture
