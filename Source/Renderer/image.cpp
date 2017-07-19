#include "image.h"

//TODO: update code commentor script to handle namespaces
namespace texture {

Image::Image(std::unique_ptr<unsigned char[]> data, int width, int height) 
	: m_data(std::move(data)), m_width(width), m_height(height) {}

Image::~Image() {}

int Image::getWidth() const {
	return m_width;
}

int Image::getHeight() const {
	return m_height;
}

unsigned char * Image::getData() const {
	return m_data.get();
}

void Image::flipVertically() {
	const int width = m_width * 3; // Each pixel has RGB bytes

	// Create temp pointer to store the changes
	auto temp = std::make_unique<unsigned char[]>(width * m_height);
	
	// Read from current data to temp in reverse row order
	for (int from = width * (m_height - 1), to = 0; 
		from >= 0; 
		from -= width, to += width) {
		// Read bytes of one row
		for (int i = 0; i < width; ++i) {
			temp[to + i] = m_data[from + i];
		}
	}

	// Replace the old data with altered temp
	m_data = std::move(temp);
}

void Image::flipHorizontally() {
	const int width = m_width * 3; // Each pixel has RGB bytes

	// Create temp pointer to store the changes
	auto temp = std::make_unique<unsigned char[]>(width * m_height);

	// Process rows
	for (int i = 0; i < m_height; ++i) {
		// Invert bytes within one row
		int from = (i + 1) * width - 3;
		int to = i * width;
		for (int j = 0; j < width; j += 3) {
			temp[to++] = m_data[from++];
			temp[to++] = m_data[from++];
			temp[to++] = m_data[from++];
			from -= 6; // Move to start of previous pixel
		}
	}

	// Replace the old data with altered temp
	m_data = std::move(temp);
}

} // namespace texture