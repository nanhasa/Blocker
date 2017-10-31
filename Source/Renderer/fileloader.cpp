#include "Renderer/fileloader.h"

#include <algorithm>
#include <cctype>
#include <fstream>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

#include "Renderer/bmp.h"
#include "Utility/contract.h"
#include "Utility/locator.h"
#include "Utility/staticsafelogger.h"
#include "Utility/utility.h"


namespace fileloader {

	//Anonymous namespace to hide factory method from namespace interface
	namespace {

		StaticSafeLogger g_log("Texture");

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
			if (size > Locator::getConfig()->get("MaxByteFileSizeToLoad", 5120000)) {
				g_log.error("File is too big to loadTexture : " + utility::toStr(size) + " bytes");
				return false;
			}

			return true;
		}

		/**
		* \brief Recognizes file type from file extension and returns correct image type to loadTexture the file
		* \param filename File name without file path
		* \return Pointer to ImageType matching the parameter file extension, nullptr if file is not supported
		*/
		std::unique_ptr<IImageType> getImageType(const std::string& filename)
		{
			const std::size_t found = filename.find_last_of(".");
			if (found == std::string::npos || found == filename.length()) {
				g_log.error("Could not recognize file type from file extension");
				return nullptr;
			}

			std::string ext = filename.substr(found + 1);
			std::for_each(ext.begin(), ext.end(), ::tolower);
			if (ext == "bmp")
				return std::make_unique<BMP>(g_log);

			g_log.error("Extension " + ext + " is not supported file type");
			return nullptr;
		}


	} // anonymous namespace


	std::unique_ptr<Image> loadTexture(const std::string& file)
	{
		REQUIRE(!file.empty());
		if (file.empty()) {
			g_log.error("Load(): No filename was provided");
			return nullptr;
		}

		g_log.info("Loading file " + file);
		std::ifstream stream(
			Locator::getConfig()->get("DataPath", std::string("../Data/")) + "Images/" + file, 
			std::ios::binary);

		if (!validateFile(stream)) {
			g_log.error("Could not open file " + file);
			return nullptr;
		}

		auto type = getImageType(file);
		type->vLoadFile(stream);
		stream.close();
		return std::make_unique<Image>(std::move(type));
	}

	// TODO: this function is still not finished
	bool loadModel(const std::string& file, std::vector<Mesh>& meshes, bool useFlatShading)
	{
		//REQUIRE(!file.empty());
		//if (file.empty()) {
		//	g_log.error("loadMode(): file parameter empty");
		//	return false;
		//}
		//
		//// Open stream
		//std::ifstream stream(Locator::getConfig()->get("DataPath", std::string("../Data/")) + "Models/" + file);
		//if (!stream.is_open()) {
		//	g_log.error("loadModel(): Could not open file: " + file);
		//	return false;
		//}
		//
		//// Clear reference parameters
		//glm::vec3 vertices;
		//glm::vec2 uvs;
		//glm::vec3 normals;
		//(void)useFlatShading; // Not used just yet TODO: implement useFlatShading
		(void)meshes; 
		//
		//// Loop through file
		//std::string line;
		//for (int row = 1; std::getline(stream, line); ++row) {
		//	std::string header = line.substr(0, 2);
		//	if (header == "v ") {
		//		
		//	}
		//	else if (header[0] == 'f') {
		//		
		//	}
		//	else if (header[0] == '#') {
		//		
		//	}
		//}
		//return false;

		(void)useFlatShading;
		(void)file;


		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		std::vector<unsigned int> indices = {
			// note that we start from 0!
			0, 1, 3,
			// first Triangle
			1, 2, 3 // second Triangle
		};

		std::vector<Vertex> mesh;
		for (int i = 0; i < 180; ) {
			
			const float v1 = vertices[i++];
			const float v2 = vertices[i++];
			const float v3 = vertices[i++];
			const glm::vec3 vert(v1, v2, v3);
		
			const float uv1 = vertices[i++];
			const float uv2 = vertices[i++];
			const glm::vec2 uv(uv1, uv2);
		
			Vertex v;
			v.position = vert;
			v.normal = glm::vec3();
			v.uvCoord = uv;
		
			mesh.emplace_back(std::move(v));
			
		}
		meshes.emplace_back(Mesh(std::move(mesh), std::move(indices)));
		return true;
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

} // namespace fileloader
