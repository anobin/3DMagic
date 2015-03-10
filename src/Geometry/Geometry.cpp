#include <Geometry\Geometry.h>
#include <Geometry\Sphere.h>

namespace Magic3D
{

const Sphere& Geometry::getBoundingSphere() const
{
    return this->getCollisionShape().getBoundingSphere();
}


};