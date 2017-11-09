#pragma once

#include "Object/terrain.h"
#include "Renderer/modelmanager.h"

enum TERRAIN_TYPE { GRASS };

namespace terrainFactory {

	Terrain createCube(const TERRAIN_TYPE type, const Transform& transform, ModelManager& modelManager);

	/**
	 * \brief createCube
	 * \param objects
	 * \param count
	 * \param modelManager
	 * \post !objects.empty()
	 * \return
	 */
	bool initializeWorld(std::vector<Terrain>& objects, const unsigned int count, ModelManager& modelManager);
}
