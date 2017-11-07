#pragma once

#include "Object/object.h"
#include "Object/renderable.h"

class Terrain : public Object {
public:

	/**
	 * \brief Constructor with default starting transform
	 * \param model
	 * \param textureId
	 */
	Terrain(std::shared_ptr<Model> model, unsigned int textureId);

	/**
	 * \brief Constructor with custom starting transform
	 * \param model
	 * \param textureId
	 * \param transform
	 */
	Terrain(std::shared_ptr<Model> model, unsigned int textureId,
		const Transform& transform);

	~Terrain() = default;

	/**
	 * \brief onUpdate Called on every tick to render object
	 * \param renderer Reference to renderer
	 * \param deltatime Time since last tick
	 */
	void onUpdate(IRenderer& renderer, const float deltatime) override;

private:
	Renderable m_renderable; //!< Used to render object
};
