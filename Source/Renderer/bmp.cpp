#include "Renderer/bmp.h"

#include <fstream>

#include "Renderer/fileloader.h"
#include "Utility/contract.h"
#include "Utility/utility.h"

BMP::BMP(StaticSafeLogger& log) : m_fileheader(nullptr), m_infoheader(nullptr), m_data(nullptr), m_log(&log) {}

BMP::~BMP() {}

bool BMP::vLoadFile(std::ifstream& stream)
{
	REQUIRE(stream.is_open());
	if (!stream.is_open()) {
		m_log->error("vLoadFile", "Could not read BMP File, because stream provided is not open");
		return false;
	}

	// Test if file is long enough to have headers
	stream.seekg(std::ios::beg); // Make sure the cursor is at the beginning of file
	const auto size = fileloader::getFileSize(stream);
	if (size < 54) {
		// Offset of BMP file
		m_log->error("vLoadFile", "Cannot read file, because file is smaller than BMP header size (54B): " 
			+ utility::toStr(size) + "B");
		return false;
	}

	// Allocate byte memory that will hold the two headers
	auto header = std::make_unique<uint8_t[]>(sizeof(BITMAPFILEHEADER));
	auto info = std::make_unique<uint8_t[]>(sizeof(BITMAPINFOHEADER));

	// Read file into header buffers
	stream.read(reinterpret_cast<char*>(header.get()), sizeof(BITMAPFILEHEADER));
	stream.read(reinterpret_cast<char*>(info.get()), sizeof(BITMAPINFOHEADER));

	// Construct headers
	m_fileheader = std::unique_ptr<BITMAPFILEHEADER>(reinterpret_cast<BITMAPFILEHEADER*>(header.release()));
	m_infoheader = std::unique_ptr<BITMAPINFOHEADER>(reinterpret_cast<BITMAPINFOHEADER*>(info.release()));

	// Check if the file is a BMP file
	if (m_fileheader->bfType != BF_TYPE_MB) {
		m_log->error("vLoadFile", "File does not contain BMP file");
		m_fileheader = nullptr;
		m_infoheader = nullptr;
		return false;
	}

	// Check if file is 24 bits per pixel
	if (m_infoheader->biBitCount != BIT_COUNT_24) {
		m_log->error("vLoadFile", "Cannot read file, because only 24bit BMP files are supported");
		m_fileheader = nullptr;
		m_infoheader = nullptr;
		return false;
	}

	// Check if the header shows the size of the image
	// If the size in header is zero, use calculated value based on offset to data and file size
	unsigned int imageSize = m_infoheader->biSizeImage;
	if (imageSize == 0) {
		m_log->warn("vLoadFile", "Warning! BMP file header does not show the size of the file");
		imageSize = m_fileheader->bfSize - m_fileheader->bfOffBits;
		m_infoheader->biSizeImage = imageSize;
	}

	// Allocate pixel memory
	m_data = std::make_unique<uint8_t[]>(imageSize);

	// Go to where image data starts
	stream.seekg(m_fileheader->bfOffBits);

	// Read image data
	stream.read(reinterpret_cast<char*>(m_data.get()), imageSize);

	ENSURE(m_fileheader != nullptr);
	ENSURE(m_infoheader != nullptr);
	ENSURE(m_data != nullptr);
	m_log->info("vLoadFile", "BMP file load was successful");

	return true;
}

std::unique_ptr<uint8_t[]> BMP::vDecode()
{
	REQUIRE(m_fileheader != nullptr);
	REQUIRE(m_infoheader != nullptr);
	REQUIRE(m_data != nullptr);

	if (m_fileheader == nullptr || m_infoheader == nullptr || m_data == nullptr) {
		m_log->error("vDecode", "BMP not properly initialized before calling decode");
		return nullptr;
	}

	// Allocate memory for copy of data
	const unsigned int width = m_infoheader->biWidth;
	const unsigned int height = m_infoheader->biHeight;
	const unsigned int imageSize = width * height * 3;

	auto decode = std::make_unique<uint8_t[]>(imageSize); // Create pointer to copied data
	unsigned int pad = 4 - m_infoheader->biWidth % 4; // Padding bytes count
	if (pad == 4) pad = 0;

	// Copy data
	for (unsigned int i = 0, row = 0, w = width * 3; row < height; ++row) {
		for (unsigned int j = 0; j < w; j += 3 , i += 3) {
			// 3 bytes per pixel
			// Change BGR format to RGB and ignore padding
			decode[i] = std::move(m_data[i + 2 + row * pad]);
			decode[i + 1] = std::move(m_data[i + 1 + row * pad]);
			decode[i + 2] = std::move(m_data[i + row * pad]);
		}
	}
	return std::move(decode);
}

int BMP::vGetHeight() const
{
	REQUIRE(m_infoheader != nullptr);
	if (m_infoheader == nullptr) {
		m_log->error("vGetHeight", "BMP not properly initialized before calling getHeight");
		return 0;
	}

	return m_infoheader->biHeight;
}

int BMP::vGetWidth() const
{
	REQUIRE(m_infoheader != nullptr);
	if (m_infoheader == nullptr) {
		m_log->error("vGetWidth", "BMP not properly initialized before calling getWidth");
		return 0;
	}

	return m_infoheader->biWidth;
}
