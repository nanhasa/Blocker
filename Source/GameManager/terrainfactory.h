#pragma once

#include <memory>

#include "Object/terrain.h"
#include "Renderer/modelmanager.h"

enum TERRAIN_TYPE { GRASS };

namespace terrainFactory {

	std::unique_ptr<Terrain> createCube(const TERRAIN_TYPE type, const Transform& transform, ModelManager& modelManager);

	/**
	 * \brief createCube
	 * \param objects
	 * \param count
	 * \param modelManager
	 * \post !objects.empty()
	 * \return
	 */
	bool initializeWorld(std::vector<std::unique_ptr<Terrain>>& objects, const unsigned int count, ModelManager& modelManager);
}
