#pragma once

#include <map>
#include <memory>
#include <string>

#include "Renderer/model.h"

class ModelManager {
public:

	/**
	 * \brief ModelManager
	 */
	ModelManager();

	/**
	 * \brief ~ModelManager
	 */
	~ModelManager();

	/**
	 * \brief getModel
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
	std::map<std::string, std::shared_ptr<Model>> m_models;
	std::map<std::string, unsigned int> m_textureIds;
};
