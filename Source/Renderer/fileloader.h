#pragma once

#include <memory>
#include <string>

#include "interfaces.h"
#include "Renderer/image.h"
#include "Renderer/mesh.h"

// Namespace to group image loading functionality
namespace fileloader {

	/**
	 * \brief Load file and get image RGB byte array
	 * \param file Filename without filepath
	 * \pre !file.empty()
	 * \return Pointer to Image which holds image RGB byte array, width, and height
	 */
	std::unique_ptr<Image> loadTexture(const std::string& file);

	/**
	 * \brief Used to load 3D model file (.obj) and to create a 
	 * \param file Filename of model
	 * \param meshes Out parameter used to load data from file
	 * \pre !file.empty()
	 * \return True if successful, otherwise false
	 */
	bool loadModel(const std::string& file, std::vector<Mesh>& meshes);

	/**
	 * \brief Get byte size of file
	 * \param stream Filestream to the file
	 * \pre stream.is_open()
	 * \post originalPosition == stream.tellg()
	 * \return File size in bytes
	 */
	std::streampos getFileSize(std::ifstream & stream);

} // namespace fileloader
