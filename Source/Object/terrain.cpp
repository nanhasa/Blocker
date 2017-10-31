#include "terrain.h"

Terrain::Terrain(ModelManager& modelManager) : Object(), m_renderable(modelManager, "square.bmp", "cube") {}

Terrain::Terrain(ModelManager& modelManager, const Transform & transform) 
	: Object(transform), m_renderable(modelManager, "square.bmp", "cube") {}

void Terrain::onUpdate(IRenderer & renderer, const float deltatime)
{
	(void)deltatime;
	m_renderable.onUpdate(renderer, transform);
}
