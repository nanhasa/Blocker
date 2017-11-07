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
	 * \return
	 */
	std::shared_ptr<Model> getModel(const std::string& modelFilename);

	/**
	 * \brief getTextureId
	 * \param textureFilename
	 * \pre !textureFilename.empty()
	 * \return
	 */
	unsigned int getTextureId(const std::string& textureFilename);

private:
	std::map<std::string, std::shared_ptr<Model>> m_models;	//!< Map pairing filename and pointer to model
	std::map<std::string, unsigned int> m_textureIds;		//!< Map pairing filename and texture id
	Logger m_log;											//!< Logger
};
