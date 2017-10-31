#pragma once

#include "interfaces.h"
#include "Object/transform.h"

class Object {
public:
	Transform transform;

	virtual void onUpdate(IRenderer& renderer, const float deltatime) = 0;

protected:
	Object() = default;
	explicit Object(const Transform& transform) : transform(transform) {}
	virtual ~Object() = default;
};