#include "Renderer/fileloader.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

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

		StaticSafeLogger g_log("FileLoader");

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
				g_log.warn("validateFile", "Stream not open in validateFile");
				return false;
			}

			if (isStreamEmpty(stream)) {
				g_log.warn("validateFile", "File empty in validateFile");
				return false;
			}

			const auto size = getFileSize(stream);
			if (size > Locator::getConfig()->get("MaxByteFileSizeToLoad", 5120000)) {
				g_log.error("validateFile", "File is too big: " + utility::toStr(size) + " bytes");
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
				g_log.error("getImageType", "Could not recognize file type from file extension");
				return nullptr;
			}

			std::string ext = filename.substr(found + 1);
			std::for_each(ext.begin(), ext.end(), ::tolower);
			if (ext == "bmp")
				return std::make_unique<BMP>(g_log);

			g_log.error("getImageType", "Extension " + ext + " is not supported file type");
			return nullptr;
		}


	} // anonymous namespace


	std::unique_ptr<Image> loadTexture(const std::string& file)
	{
		REQUIRE(!file.empty());
		if (file.empty()) {
			g_log.error("loadTexture", "No filename was provided");
			return nullptr;
		}

		g_log.info("Loading file " + file);
		std::ifstream stream(
			Locator::getConfig()->get("DataPath", std::string("../Data/")) + "Images/" + file, 
			std::ios::binary);

		if (!validateFile(stream)) {
			g_log.error("loadTexture", "Could not open file " + file);
			return nullptr;
		}

		auto type = getImageType(file);
		type->vLoadFile(stream);
		stream.close();
		return std::make_unique<Image>(std::move(type));
	}

	bool loadModel(const std::string& file, std::vector<Mesh>& meshes)
	{
		REQUIRE(!file.empty());
		if (file.empty()) {
			g_log.error("loadModel", "file parameter empty");
			return false;
		}
		
		// Open stream
		std::ifstream stream(
			Locator::getConfig()->get("DataPath", std::string("../Data/")) + "Models/" + file);
		
		// Validate stream
		if (!validateFile(stream)) {
			g_log.error("loadModel", "Could not load file: " + file);
			return false;
		}

		// Create temp vectors to read contents from file
		std::vector<glm::vec3> tempVertices;
		std::vector<glm::vec2> tempUVs;
		std::vector<glm::vec3> tempNormals;

		// Vectors for indices
		std::vector<unsigned short> vertexIndices;
		std::vector<unsigned short> uvIndices;
		std::vector<unsigned short> normalIndices;
		
		// Loop through file
		std::string line;
		for (int row = 1; std::getline(stream, line); ++row) {
			if (line.length() < 2) // Row is not legit if it is shorter than 2 characters 
				continue;

			auto header = line.substr(0, 2);
			if (header[0] == '#') 
				continue; // Comment line, do nothing
			
			std::stringstream ss(line.substr(2));
			try {
				if (header == "vt") { // uv data
					glm::vec2 v2;
					ss >> v2.x;
					ss >> v2.y;
					tempUVs.emplace_back(std::move(v2));
				}
				else if (header == "vn") { // normal data
					glm::vec3 v3;
					ss >> v3.x;
					ss >> v3.y;
					ss >> v3.z;
					tempNormals.emplace_back(std::move(v3));
				}
				else if (header[0] == 'v') { // vertice data
					glm::vec3 v3;
					ss >> v3.x;
					ss >> v3.y;
					ss >> v3.z;
					tempVertices.emplace_back(std::move(v3));
				}
				else if (header[0] == 'f') { // face data
					for (int i = 0; i < 3; ++i) { // For triangulated data there are 3 sets of 3 values in one row
						unsigned short vertexId, uvId, normalId;
						ss >> vertexId;
						ss.ignore(1); // Ignore '/' character
						ss >> uvId;
						ss.ignore(1); // Ignore '/' character
						ss >> normalId;

						vertexIndices.emplace_back(vertexId);
						uvIndices.emplace_back(uvId);
						normalIndices.emplace_back(normalId);
					}
				}
			}
			catch(std::ios_base::failure f) {
				g_log.error("loadModel", "Bad row " 
					+ utility::toStr(row) + " in file " + file + ". What: " + f.what());
				return false;
			}
		}

		// File has been read now
		// Process the three indexes into one
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;
		for (unsigned int i = 0; i < vertexIndices.size(); ++i) {
			Vertex vert;
			vert.position = tempVertices[vertexIndices[i] - 1]; // indexes in .obj start from 1
			vert.normal = tempNormals[normalIndices[i] - 1];
			vert.uvCoord = tempUVs[uvIndices[i] - 1];

			// Search for existing vertex
			auto it = std::find_if(
				vertices.begin(), 
				vertices.end(), 
				[vert](const auto rhs) { return rhs.isEqual(vert); });

			if (it == vertices.end()) {
				// No unique vertex + uv + normal combo found
				// add it to vertices and indices
				vertices.emplace_back(std::move(vert));
				indices.emplace_back(static_cast<unsigned short>(vertices.size() - 1)); // OpenGL wants indices to start from 0
			}
			else {
				// Unique combo found
				// Add it to indices only
				indices.emplace_back(static_cast<unsigned short>(std::distance(vertices.begin(), it)));
			}
		}
		meshes.clear();
		meshes.emplace_back(std::move(vertices), std::move(indices));
		g_log.info("modelLoader", "Succesfully loaded file: " + file);
		return true;
	}

	std::streampos getFileSize(std::ifstream& stream)
	{
		REQUIRE(stream.is_open());
		if (!stream.is_open()) {
			g_log.error("getFileSize", "Not valid stream in getFileSize");
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
