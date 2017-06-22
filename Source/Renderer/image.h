#pragma once

#include <memory>

class Image {
public:	
	Image(std::unique_ptr<unsigned char[]> data, int width, int height);
	~Image();

	int getWidth() const;
	int getHeight() const;
	unsigned char* getData() const;

	void flipVertically();
	void flipHorizontally();

private:
	std::unique_ptr<unsigned char[]> m_data;
	int m_width;
	int m_height;
};