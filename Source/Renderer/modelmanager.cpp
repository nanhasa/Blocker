#include "Renderer/fileloader.h"
#include "Renderer/image.h"
#include "Renderer/modelmanager.h"
#include "Utility/contract.h"

ModelManager::ModelManager() : m_log("ModelManager") {}

ModelManager::~ModelManager()
{
	for (const auto pair : m_textureIds) {
		glDeleteTextures(1, &pair.second);
	}
}

std::shared_ptr<Model> ModelManager::getModel(const std::string & modelFilename)
{
	REQUIRE(!modelFilename.empty());
	if (modelFilename.empty()) {
		m_log.error("getTextureId", "No filename provided");
		return nullptr;
	}

	const auto it = m_models.find(modelFilename);
	if (it == m_models.end()) {
		std::vector<Mesh> meshes;
		if (!fileloader::loadModel(modelFilename, meshes)) {
			m_log.error("getModel", "Error loading file: " + modelFilename);
			return nullptr;
		}

		m_log.info("getModel", "Successfully loaded file: " + modelFilename);
		m_models[modelFilename] = std::make_shared<Model>(std::move(meshes));
		return m_models[modelFilename];
	}
	return it->second;
}

unsigned int ModelManager::getTextureId(const std::string & textureFilename)
{
	REQUIRE(!textureFilename.empty());
	if (textureFilename.empty()) {
		m_log.error("getTextureId", "No filename provided");
		return 0;
	}

	const auto it = m_textureIds.find(textureFilename);
	// If texture is not loaded yet, load it
	if (it == m_textureIds.end()) {
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load fileloader data
		std::unique_ptr<Image> txrData = fileloader::loadTexture(textureFilename);
		if (txrData == nullptr) {
			m_log.error("getTextureId", "Error loading file: " + textureFilename);
			return 0;
		}

		const unsigned int width = txrData->getWidth();
		const unsigned int height = txrData->getHeight();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, txrData->getData());
		glGenerateMipmap(GL_TEXTURE_2D);
		txrData.reset();

		m_log.info("getTextureId", "Successfully loaded file: " + textureFilename);
		m_textureIds[textureFilename] = textureId;
		return m_textureIds[textureFilename];
	}
	// Return texture that has already been loaded
	return it->second;
}
