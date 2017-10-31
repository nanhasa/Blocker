#pragma once

#include "Object/object.h"
#include "Object/renderable.h"

class Terrain : public Object {
public:

	/**
	 * \brief Terrain
	 * \param modelManager
	 */
	explicit Terrain(ModelManager& modelManager);

	/**
	 * \brief Terrain
	 * \param modelManager
	 * \param transform
	 */
	Terrain(ModelManager& modelManager, const Transform& transform);
	~Terrain() = default;

	/**
	 * \brief onUpdate
	 * \param renderer
	 * \param deltatime
	 */
	void onUpdate(IRenderer& renderer, const float deltatime) override;

private:
	Renderable m_renderable;
};
