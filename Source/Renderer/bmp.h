#pragma once

#include "Interfaces.h"

#include <memory>
#include <string>


// Without the pragma, the unsigned short fields are being padded to 4 bytes.
// The size of BITMAPFILEHEADER (w/o pragma) is 20, but in the file it is written sequentially as 14 bytes.
#pragma pack(2)

struct BITMAPFILEHEADER {			 /* BMP file header structure */
	unsigned short bfType;           // Number for file
	unsigned int   bfSize;           // Size of file
	unsigned short bfReserved1;      // Reserved
	unsigned short bfReserved2;
	unsigned int   bfOffBits;        // Offset to bitmap data
};

#pragma pack()

#define BF_TYPE_MB 0x4D42			 // MB - shows that file is BMP file
#define BIT_COUNT_24 24				 // Only 24 bits per pixel supported
#define BI_RGB 0x0000				 // The bitmap is in uncompressed red green blue (RGB) format that is not compressed and does not use color masks.
#define PIXELS_PER_METER 2835		 // 72 DPI × 39.3701 inches per meter yields 2834.6472
#define NUM_OF_PLANES 1				 // Always 1

struct BITMAPINFOHEADER {			 /* BMP file info structure */
	unsigned int   biSize;           // Size of info header
	int            biWidth;          // Width of image
	int            biHeight;         // Height of image
	unsigned short biPlanes;         // Number of color planes
	unsigned short biBitCount;       // Number of bits per pixel
	unsigned int   biCompression;    // Type of compression to use
	unsigned int   biSizeImage;      // Size of image data
	int            biXPelsPerMeter;  // X pixels per meter
	int            biYPelsPerMeter;  // Y pixels per meter
	unsigned int   biClrUsed;        // Number of colors used
	unsigned int   biClrImportant;   // Number of important colors
};

// Class used to load BMP files and decode them to RGB byte array
class BMP : public IImageType {
public:

	/**
	 * \brief Builder. Call loadFile afterwards to actually initialize.
	 */
	BMP();

	/**
	 * \brief Destructor
	 */
	~BMP();

	/**
	 * \brief Loads file content into memory
	 * \param stream filestream to bmp file
	 * \pre stream.is_open()
	 * \post m_fileheader != nullptr
	 * \post m_infoheader != nullptr
	 * \post m_data != nullptr
	 * \return true if successful, otherwise false
	 */
	bool loadFile(std::ifstream& stream) override;

	/**
	 * \brief Get decoded image data in RGB format
	 * \pre m_fileheader != nullptr
	 * \pre m_infoheader != nullptr
	 * \pre m_data != nullptr
	 * \return Pointer to decoded RGB byte array, nullptr if object not initialized with loadFile()
	 */
	std::unique_ptr<uint8_t[]> decode() override;

	/**
	 * \brief Get image height in pixels
	 * \pre m_infoheader != nullptr
	 * \return Image height, 0 if object not initialized with loadFile()
	 */
	int getHeight() const override;

	/**
	 * \brief Get image width in pixels
	 * \pre m_infoheader != nullptr
	 * \return Image width, 0 if object not initialized with loadFile()
	 */
	int getWidth() const override;

private:
	std::unique_ptr<BITMAPFILEHEADER> m_fileheader; // Header
	std::unique_ptr<BITMAPINFOHEADER> m_infoheader; // Header
	std::unique_ptr<uint8_t[]> m_data;				// Image payload byte array
};
