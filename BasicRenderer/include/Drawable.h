#pragma once

#include <memory>
#include <variant>
#include "PrimitiveTypes.h"

namespace BasicRenderer
{
	class Mesh;

	using MeshRef = std::shared_ptr<Mesh>;
	using Drawable = std::variant<Sphere, Plane, MeshRef>;
}
