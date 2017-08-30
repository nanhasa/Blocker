#include <iostream>

#include "contract.h"
#include "Renderer/bmp.h"
#include "Renderer/texture.h"

namespace texture {

	//Anonymous namespace to hide factory method from namespace interface
	namespace {

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
				std::cerr << "\tCould not open file" << std::endl;
				return false;
			}

			if (isStreamEmpty(stream)) {
				std::cerr << "\tCould not read an empty file" << std::endl;
				return false;
			}

			// Test if file is long enough to have headers
			const auto size = getFileSize(stream);
			if (size > 5120000) {
				// Hard limit the file size TODO read from config 
				std::cerr << "\tFile is too big to load: " << size << " bytes" << std::endl;
			}

			return true;
		}

		/**
		* \brief Recognizes file type from file extension and returns correct image type to load the file
		* \param filename File name without file path
		* \return Pointer to ImageType matching the parameter file extension, nullptr if file is not supported
		*/
		std::unique_ptr<IImageType> getImageType(const std::string& filename)
		{
			const std::size_t found = filename.find_last_of(".");
			if (found == std::string::npos || found == filename.length()) {
				std::cerr << "\tCould not recognize file type from file extension" << std::endl;
				return nullptr;
			}

			std::string extension = filename.substr(found + 1);

			//TODO return correct image type based on extension

			return std::make_unique<BMP>();
		}

	} // anonymous namespace


	Image::Image(std::unique_ptr<uint8_t[]> data, int width, int height)
		: m_data(std::move(data)), m_width(width), m_height(height) {}

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
		const int width = m_width * 3; // Each pixel has RGB bytes

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
		std::cout << "Loading file " << file << std::endl;
		std::ifstream stream(R"(..\Data\Images\)" + file, std::ios::binary); //Todo read path from config 

		if (!validateFile(stream)) { return nullptr; }

		std::unique_ptr<IImageType> type = getImageType(file);
		type->vLoadFile(stream);
		stream.close();

		int width = type->vGetWidth();
		int height = type->vGetHeight();
		return std::make_unique<Image>(type->vDecode(), width, height);
	}

	std::streampos getFileSize(std::ifstream& stream)
	{
		REQUIRE(stream.is_open());

		if (!stream) {
			std::cout << "\tNot valid stream in getFileSize" << std::endl;
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
