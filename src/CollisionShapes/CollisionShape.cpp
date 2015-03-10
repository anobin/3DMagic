#include <CollisionShapes/CollisionShape.h>

#include <Geometry\Sphere.h>

namespace Magic3D
{
    
    
const Sphere& CollisionShape::getBoundingSphere() const
{
    if (this->boundingSphere != nullptr)
        return *this->boundingSphere;

    btVector3 bt_center;
    btScalar bt_radius;
    shape->getBoundingSphere(bt_center, bt_radius);
    
    this->boundingSphere = std::make_shared<Sphere>((Scalar)bt_radius);
    boundingSphere->translate(Vector3(bt_center.getX(), bt_center.getY(), bt_center.getZ()));
    return *boundingSphere;
}
    
    
    
    
    
    
    
};










