#pragma once

#include <vector>

#include "Object/terrain.h"
#include "Renderer/modelmanager.h"

class WorldManager {
public:

	/**
	 * \brief WorldManager
	 * \param modelManager
	 */
	WorldManager(ModelManager& modelManager);
	~WorldManager() = default;

	/**
	 * \brief onUpdate
	 * \param renderer
	 * \param deltatime
	 */
	void onUpdate(IRenderer& renderer, const float deltatime);

private:
	std::vector<Terrain> m_objects;
};
