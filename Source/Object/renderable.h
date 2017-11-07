#pragma once

#include "interfaces.h"
#include "Object/transform.h"

class Renderable {
public:

	/**
	 * \brief Constructor
	 * \param model Pointer to model object holding the vertex data
	 * \param textureId OpenGL id for the texture
	 */
	Renderable(std::shared_ptr<Model> model, unsigned int textureId);

	~Renderable() = default;

	/**
	 * \brief onUpdate Called on every tick to render object
	 * \param renderer Reference to renderer
	 * \param transform Reference to the transform of the object renderer
	 */
	void onUpdate(IRenderer& renderer, Transform& transform) const;

private:
	std::shared_ptr<Model> m_model;	//!< Pointer to the model holding the vertex data
	unsigned int m_textureId;		//!< OpenGL texture id
};
