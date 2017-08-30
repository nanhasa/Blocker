#include <fstream>
#include <iostream>

#include "contract.h"
#include "Renderer/bmp.h"
#include "Renderer/texture.h"

BMP::BMP() {
	m_data = nullptr;
	m_fileheader = nullptr;
	m_infoheader = nullptr;
}

BMP::~BMP() {
}

bool BMP::vLoadFile(std::ifstream& stream) {
	REQUIRE(stream.is_open());

	if (!stream.is_open()) {
		std::cout << "\tCould not read BMP File, because stream provided is not open" << std::endl;
		return false;
	}

	// Test if file is long enough to have headers
	stream.seekg(std::ios::beg); // Make sure the cursor is at the beginning of file
	const auto size = texture::getFileSize(stream);
	if (size < 54) { // Offset of BMP file
		std::cerr << "\tCannot read file, because file is smaller than BMP header size (54B): " 
			<< size << "B" << std::endl;
	}

	// Allocate byte memory that will hold the two headers
	auto header = std::make_unique<uint8_t[]>(sizeof(BITMAPFILEHEADER));
	auto info = std::make_unique<uint8_t[]>(sizeof(BITMAPINFOHEADER));

	// Read file into header buffers
	stream.read((char*)header.get(), sizeof(BITMAPFILEHEADER));
	stream.read((char*)info.get(), sizeof(BITMAPINFOHEADER));
	
	// Construct headers
	m_fileheader = std::unique_ptr<BITMAPFILEHEADER>((BITMAPFILEHEADER*)header.release());
	m_infoheader = std::unique_ptr<BITMAPINFOHEADER>((BITMAPINFOHEADER*)info.release());

	// Check if the file is a BMP file
	if (m_fileheader->bfType != BF_TYPE_MB) {
		std::cerr << "\tFile does not contain BMP file" << std::endl;
		m_fileheader = nullptr;
		m_infoheader = nullptr;
		return false;
	}

	// Check if file is 24 bits per pixel
	if (m_infoheader->biBitCount != BIT_COUNT_24) {
		std::cerr << "\tCannot read file, because only 24bit BMP files are supported" << std::endl;
		m_fileheader = nullptr;
		m_infoheader = nullptr;
		return false;
	}

	// Check if the header shows the size of the image
	// If the size in header is zero, use calculated value based on offset to data and file size
	unsigned int imageSize = m_infoheader->biSizeImage;
	if (imageSize == 0) {
		std::cerr << "\tWarning! File header does not show the size of the file" << std::endl;
		imageSize = m_fileheader->bfSize - m_fileheader->bfOffBits;
		m_infoheader->biSizeImage = imageSize;
	}

	// Allocate pixel memory
	m_data = std::make_unique<uint8_t[]>(imageSize);

	// Go to where image data starts
	stream.seekg(m_fileheader->bfOffBits);

	// Read image data
	stream.read((char*)m_data.get(), imageSize);

	ENSURE(m_fileheader != nullptr);
	ENSURE(m_infoheader != nullptr);
	ENSURE(m_data != nullptr);
	std::cout << "\tFile load was successful" << std::endl;

	return true;
}

std::unique_ptr<uint8_t[]> BMP::vDecode() {
	REQUIRE(m_fileheader != nullptr);
	REQUIRE(m_infoheader != nullptr);
	REQUIRE(m_data != nullptr);

	if (m_fileheader == nullptr || m_infoheader == nullptr || m_data == nullptr) {
		std::cerr << "\tImagetype not properly initialized before calling decode" << std::endl;
		return nullptr;
	}
	
	// Allocate memory for copy of data
	const unsigned int width = m_infoheader->biWidth;
	const unsigned int height = m_infoheader->biHeight;
	const unsigned int imageSize = width * height * 3;
	
	// Padding bytes count
	const unsigned int pad = 4 - m_infoheader->biWidth % 4;
	if (pad == 4) {
		// No need to remove padding so optimize the decode from BGR to RGB
		for (unsigned int i = 0; i < imageSize; i += 3) {
			std::swap(m_data[i], m_data[i + 2]);
		}
		m_fileheader.reset();
		m_infoheader.reset();
		return std::move(m_data);
	}
	
	// Copy data
	auto decode = std::make_unique<uint8_t[]>(imageSize);
	for (unsigned int i = 0, row = 0, w = width * 3; row < height; ++row) {
		for (unsigned int j = 0; j < w; j += 3, i += 3) { // 3 bytes per pixel
			// Change BGR format to RGB and ignore padding
			decode[i] = std::move(m_data[i + 2 + row * pad]);
			decode[i + 1] = std::move(m_data[i + 1 + row * pad]);
			decode[i + 2] = std::move(m_data[i + row * pad]);
		}
	}

	m_fileheader.reset();
	m_infoheader.reset();
	return std::move(decode);
}

int BMP::vGetHeight() const {
	REQUIRE(m_infoheader != nullptr);
	
	if (m_infoheader == nullptr) {
		std::cerr << "\tImagetype not properly initialized before calling getHeight" << std::endl;
		return 0;
	}
	
	return m_infoheader->biHeight;
}

int BMP::vGetWidth() const {
	REQUIRE(m_infoheader != nullptr);
	
	if (m_infoheader == nullptr) {
		std::cerr << "\tImagetype not properly initialized before calling getWidth" << std::endl;
		return 0;
	}

	return m_infoheader->biWidth;
}
