#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "interfaces.h"

// Namespace to group image loading functionality
namespace texture {

	// Class used to return from texture namespace with function load()
	class Image {
	public:

		/**
		 * \brief Builder
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
		 * \brief getWidth
		 * \return Image width in pixels
		 */
		int getWidth() const;

		/**
		 * \brief getHeight
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
		std::unique_ptr<uint8_t[]> m_data;
		const int m_width;
		const int m_height;
	};

	/**
	 * \brief Load file and get image RGB byte array
	 * \param file Filename without filepath
	 * \return Pointer to texture::Image which holds image RGB byte array, width, and height
	 */
	std::unique_ptr<Image> load(const std::string& file);

	/**
	 * \brief Tests if stream is empty or at the end
	 * \param stream Filestream to a file
	 * \return True if stream is empty or at the end, otherwise false
	 */
	bool isStreamEmpty(std::ifstream& stream);

	/**
	 * \brief Tests that stream is open, is not empty, and file is not too big (limit from config)
	 * \param stream Filestream to be tested
	 * \return True if stream is valid, otherwise false
	 */
	bool validateFile(std::ifstream& stream);

	/**
	 * \brief Get byte size of file
	 * \param stream Filestream to the file
	 * \pre stream.is_open()
	 * \return File size in bytes
	 */
	std::streampos getFileSize(std::ifstream& stream);

namespace factory {

	/**
	 * \brief Recognizes file type from file extension and returns correct image type to load the file
	 * \param filename File name without file path
	 * \return Pointer to ImageType matching the parameter file extension, nullptr if file is not supported
	 */
	std::unique_ptr<IImageType> getImageType(const std::string& filename);

} // namespace factory
} // namespace texture
