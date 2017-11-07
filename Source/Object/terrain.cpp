#include "terrain.h"

Terrain::Terrain(std::shared_ptr<Model> model, unsigned int textureId) 
	: Object(), m_renderable(model, textureId) {}

Terrain::Terrain(std::shared_ptr<Model> model, unsigned int textureId, const Transform& transform)
	: Object(transform), m_renderable(model, textureId) {}

void Terrain::onUpdate(IRenderer& renderer, const float deltatime)
{
	(void)deltatime;
	m_renderable.onUpdate(renderer, transform);
}
