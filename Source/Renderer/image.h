#pragma once

#include <memory>

#include "interfaces.h"

// Class used to return from fileloader namespace with function loadTexture()
class Image {
public:

	/**
	 * \brief Constructor. Creates valid object
	 * \param data Image RGB byte array
	 * \param width Image width in pixels
	 * \param height Image height in pixels
	 */
	Image(std::unique_ptr<uint8_t[]> data, int width, int height);

	/**
	 * \brief Constructor. Creates valid object
	 * \param imagetype Pointer to initialized imagetype
	 */
	explicit Image(std::unique_ptr<IImageType> imagetype);

	/**
	 * \brief Destructor
	 */
	~Image();

	/**
	 * \brief used to get image width in pixels
	 * \return Image width in pixels
	 */
	int getWidth() const;

	/**
	 * \brief Used to get image height in pixels
	 * \return Image height in pixels
	 */
	int getHeight() const;

	/**
	 * \brief Get image RGB byte array data
	 * \return raw pointer to data. Does not pass ownership.
	 */
	uint8_t* getData() const;

	/**
	 * \brief Reverses image vertically
	 */
	void flipVertically();

	/**
	 * \brief Reverses image horizontally
	 */
	void flipHorizontally();

private:
	std::unique_ptr<uint8_t[]> m_data;	//!< Pointer to array of image rgb bytes
	const int m_width;					//!< Image pixel width
	const int m_height;					//!< Image pixel height
};
