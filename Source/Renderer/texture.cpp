#include <algorithm>
#include <cctype>
#include <fstream>

#include "Renderer/bmp.h"
#include "Renderer/texture.h"
#include "Utility/contract.h"
#include "Utility/logger.h"

namespace texture {

	//Anonymous namespace to hide factory method from namespace interface
	namespace {

		Logger g_log("Texture");

		/**
		* \brief Tests if stream is empty or at the end
		* \param stream Filestream to a file
		* \return True if stream is empty or at the end, otherwise false
		*/
		bool isStreamEmpty(std::ifstream& stream)
		{
			if (!stream.is_open())
				return false;

			return stream.peek() == std::ifstream::traits_type::eof();
		}

		/**
		* \brief Tests that stream is open, is not empty, and file is not too big (limit from config)
		* \param stream Filestream to be tested
		* \return True if stream is valid, otherwise false
		*/
		bool validateFile(std::ifstream& stream)
		{
			if (!stream.is_open()) {
				g_log.warn("Stream not open in validateFile");
				return false;
			}

			if (isStreamEmpty(stream)) {
				g_log.warn("File empty in validateFile");
				return false;
			}

			const auto size = getFileSize(stream);
			if (size > 5120000) {
				// Hard limit the file size TODO read from config 
				g_log.error("File is too big to load : " + toStr(size) + " bytes");
				return false;
			}

			return true;
		}

		/**
		* \brief Recognizes file type from file extension and returns correct image type to load the file
		* \param filename File name without file path
		* \return Pointer to ImageType matching the parameter file extension, nullptr if file is not supported
		*/
		std::unique_ptr<IImageType> getImageType(const std::string& filename, const Logger& log)
		{
			const std::size_t found = filename.find_last_of(".");
			if (found == std::string::npos || found == filename.length()) {
				g_log.error("Could not recognize file type from file extension");
				return nullptr;
			}

			std::string ext = filename.substr(found + 1);
			std::for_each(ext.begin(), ext.end(), ::tolower);
			if (ext == "bmp")
				return std::make_unique<BMP>(log);

			g_log.error("Extension " + ext + " is not supported file type");
			return nullptr;
		}

	} // anonymous namespace


	Image::Image(std::unique_ptr<uint8_t[]> data, int width, int height)
		: m_data(std::move(data)), m_width(width), m_height(height) {}

	Image::Image(std::unique_ptr<IImageType> imagetype) 
		: m_data(imagetype->vDecode()), m_width(imagetype->vGetWidth()), m_height(imagetype->vGetHeight()) {}

	Image::~Image() {}

	int Image::getWidth() const { return m_width; }

	int Image::getHeight() const { return m_height; }

	uint8_t* Image::getData() const { return m_data.get(); }

	void Image::flipVertically()
	{
		// Create temp pointer to store the changes
		const int width = m_width * 3; // Each pixel has RGB bytes
		auto temp = std::make_unique<uint8_t[]>(width * m_height);

		// Read from current data to temp in reverse row order
		for (int from = width * (m_height - 1), to = 0;
		     from >= 0;
		     from -= width , to += width) {
			// Read bytes of one row
			for (int i = 0; i < width; ++i) { std::swap(temp[to + i], m_data[from + i]); }
		}

		// Replace the old data with altered temp
		m_data = std::move(temp);
	}

	void Image::flipHorizontally()
	{
		const int width = m_width * 3; // Each pixel has RGB (=3) bytes

		// Create temp pointer to store the changes
		auto temp = std::make_unique<uint8_t[]>(width * m_height);

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

	std::unique_ptr<Image> load(const std::string& file)
	{
		g_log.info("Loading file " + file);
		std::ifstream stream("../Data/Images/" + file, std::ios::binary); // TODO: read path from config 

		if (!validateFile(stream)) {
			g_log.error("Could not open file " + file);
			return nullptr;
		}

		auto type = getImageType(file, g_log);
		type->vLoadFile(stream);
		stream.close();
		return std::make_unique<Image>(std::move(type));
	}

	std::streampos getFileSize(std::ifstream& stream)
	{
		REQUIRE(stream.is_open());
		if (!stream) {
			g_log.error("Not valid stream in getFileSize");
			return 0;
		}

		const auto originalPosition = stream.tellg();
		stream.seekg(0, std::ios::end);
		const auto size = stream.tellg();
		stream.seekg(originalPosition); //Return stream position to where it was

		ENSURE(originalPosition == stream.tellg());
		return size;
	}

} // namespace texture
