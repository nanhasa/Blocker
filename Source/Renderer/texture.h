#pragma once

#include <fstream>
#include <memory>
#include <string>

// Namespace to group image loading functionality
namespace texture {

	// Class used to return from texture namespace with function load()
	class Image {
	public:

		/**
		 * \brief Builder. Creates valid object
		 * \param data Image RGB byte array
		 * \param width Image width in pixels
		 * \param height Image height in pixels
		 */
		Image(std::unique_ptr<uint8_t[]> data, int width, int height);

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
		 * \return
		 */
		uint8_t* getData() const;

		/**
		 * \brief Reverse image vertically
		 */
		void flipVertically();

		/**
		 * \brief Reverse image horizontally
		 */
		void flipHorizontally();

	private:
		std::unique_ptr<uint8_t[]> m_data;	//!< Pointer to array of image rgb bytes
		const int m_width;					//!< Image pixel width
		const int m_height;					//!< Image pixel height
	};

	/**
	 * \brief Load file and get image RGB byte array
	 * \param file Filename without filepath
	 * \return Pointer to texture::Image which holds image RGB byte array, width, and height
	 */
	std::unique_ptr<Image> load(const std::string& file);

	/**
	 * \brief Get byte size of file
	 * \param stream Filestream to the file
	 * \pre stream.is_open()
	 * \post originalPosition == stream.tellg()
	 * \return File size in bytes
	 */
	std::streampos getFileSize(std::ifstream & stream);

} // namespace texture
