#pragma once

#include "interfaces.h"
#include "Object/transform.h"
#include "Renderer/modelmanager.h"

class Renderable {
public:

	/**
	 * \brief Renderable
	 * \param modelManager
	 * \param textureFilename
	 * \param modelFilename
	 */
	Renderable(ModelManager& modelManager,
		const std::string& textureFilename, const std::string& modelFilename);

	/**
	 * \brief ~Renderable
	 */
	~Renderable();

	/**
	 * \brief onUpdate
	 * \param renderer
	 * \param transform
	 */
	void onUpdate(IRenderer& renderer, Transform& transform) const;

private:
	std::shared_ptr<Model> m_model;
	unsigned int m_textureId;
};
