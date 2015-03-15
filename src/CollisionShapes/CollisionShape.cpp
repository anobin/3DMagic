#include <CollisionShapes/CollisionShape.h>

#include <Geometry\Sphere.h>
#include <Geometry\Box.h>

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

    // translate to local offset of sphere
    boundingSphere->translate(Vector3(bt_center.getX(), bt_center.getY(), bt_center.getZ()));
    // translate to global offset of sphere
    boundingSphere->translate(this->translation);
    // rotation don't matter to a perfect sphere
    //boundingSphere->rotate(this->rotation);

    return *boundingSphere;
}
    
    
const Box& CollisionShape::getAABB() const
{
    if (this->aabb != nullptr)
        return *this->aabb;

    btTransform transform; // TODO: do we need to do anything with this transform?
    transform.setIdentity();
    btVector3 min, max;

    this->shape->getAabb(transform, min, max);

    Scalar length = Scalar((max - min).length());
    btVector3 center = (min + max) * btScalar(0.5);

    this->aabb = std::make_shared<Box>(length, length, length);
    this->aabb->translate(Vector3(center.getX(), center.getY(), center.getZ()));

    return *this->aabb;
}
    
    
};










