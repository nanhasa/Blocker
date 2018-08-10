#include "terrainfactory.h"

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

#include "Utility/contract.h"
#include "Utility/staticsafelogger.h"
#include "Utility/utility.h"

namespace terrainFactory {

StaticSafeLogger g_logger("TerrainFactory");

std::unique_ptr<Terrain> createCube(const TERRAIN_TYPE type, const Transform& transform, ModelManager& modelManager)
{
	std::string textureFile;
	std::string modelFile;
	if (type == GRASS) {
		textureFile = "grassQube.bmp";
		modelFile = "cube.obj";
	}

	const auto texture = modelManager.getTextureId(textureFile);
	if (texture == 0)
		throw std::invalid_argument("Could not create texture: " + textureFile);

	const auto model = modelManager.getModel(modelFile);
	if (model == nullptr)
		throw std::invalid_argument("Could not create model: " + modelFile);

	g_logger.info(
		"createCube", "Created terrain of type " + utility::toStr(static_cast<unsigned int>(type))
			+ " using model " + modelFile + " and texture " + textureFile
			+ " to position ("
			+ utility::toStr(transform.position.x) + ", "
			+ utility::toStr(transform.position.y) + ", "
			+ utility::toStr(transform.position.z)
			+ ") with rotation ("
			+ utility::toStr(transform.rotation.x) + ", "
			+ utility::toStr(transform.rotation.y) + ", "
			+ utility::toStr(transform.rotation.z) + ")"
	);

	return std::make_unique<Terrain>(
		model,
		texture,
		transform
	);
}

bool initializeWorld(std::vector<std::unique_ptr<Terrain>>& objects, const unsigned int count, ModelManager& modelManager)
{
	g_logger.info("initializeWorld", "Started world creation");

	std::vector<Transform> cubePositions;
	//= {
	//	Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3()),
	//	Transform(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3()),
	//	Transform(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3()),
	//	Transform(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3()),
	//	Transform(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3()),
	//	Transform(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3()),
	//	Transform(glm::vec3(2.0f, 0.0f, -1.0f), glm::vec3()),
	//	Transform(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3())
	//};

	for (float i = 0; i < 100; ++i) {
		for (float j = 0; j < 200; ++j) {
			cubePositions.emplace_back(glm::vec3(i, 0.0f, j), glm::vec3());
		}
	}

	objects.clear();
	for (unsigned int i = 0; i < count; ++i) {
		TERRAIN_TYPE type = GRASS;
		try {
			objects.emplace_back(createCube(type, cubePositions[i], modelManager));
		}
		catch (std::bad_function_call& bfc) {
			g_logger.error(
				"initializeWorld", "Error creating terrain of type "
				+ utility::toStr(static_cast<unsigned int>(type))
				+ ": " + bfc.what());
		}
	}

	ENSURE(!objects.empty());
	if (objects.empty()) {
		g_logger.fatal("initializeWorld", "Could not create any cubes");
	}

	g_logger.info("initializeWorld", "Created successfully "
		+ utility::toStr(objects.size()) + " out of " + utility::toStr(count) + " cubes");

	return true;
}

} // terrainFactory namespace
