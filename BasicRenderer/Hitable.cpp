#include "Hitable.h"
#include "Material.h"

Hitable::Hitable()
{
	material = new Material(Vector3::One()); //TODO remove
}
