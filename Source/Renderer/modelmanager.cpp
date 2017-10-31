#include "Renderer/fileloader.h"
#include "Renderer/image.h"
#include "Renderer/modelmanager.h"
#include "Utility/contract.h"

ModelManager::ModelManager()
{
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
	std::unique_ptr<Image> txrData = fileloader::loadTexture("square.bmp");
	const unsigned int width = txrData->getWidth();
	const unsigned int height = txrData->getHeight();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, txrData->getData());
	glGenerateMipmap(GL_TEXTURE_2D);
	txrData.reset();

	m_textureIds["square.bmp"] = textureId;

	std::vector<Mesh> meshes;
	fileloader::loadModel("asd", meshes, false);
	m_models["cube"] = std::make_shared<Model>(std::move(meshes));
}

ModelManager::~ModelManager() {}

std::shared_ptr<Model> ModelManager::getModel(const std::string & modelFilename)
{
	REQUIRE(!modelFilename.empty());
	return m_models[modelFilename];
}

unsigned int ModelManager::getTextureId(const std::string & textureFilename)
{
	REQUIRE(!textureFilename.empty());
	return m_textureIds[textureFilename];
}
