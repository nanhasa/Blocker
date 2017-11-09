#pragma once

#include <map>
#include <memory>
#include <string>

#include "Renderer/model.h"

class ModelManager {
public:

	/**
	 * \brief Constructor
	 */
	ModelManager();
	~ModelManager() = default;

	/**
	 * \brief Used to get pointer to 3d model
	 * \param modelFilename
	 * \pre !modelFilename.empty()
	 * \return Pointer to model, nullptr if it could not be loaded
	 */
	std::shared_ptr<Model> getModel(const std::string& modelFilename);

	/**
	 * \brief Used to get OpenGL id of texture
	 * \param textureFilename Filename
	 * \pre !textureFilename.empty()
	 * \return OpenGL texture id, 0 if texture could not be loaded
	 */
	unsigned int getTextureId(const std::string& textureFilename);

private:
	std::map<std::string, std::shared_ptr<Model>> m_models;	//!< Map pairing filename and pointer to model
	std::map<std::string, unsigned int> m_textureIds;		//!< Map pairing filename and texture id
	Logger m_log;											//!< Logger
};
