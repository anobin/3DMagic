#ifndef MAGIC3D_SPHERE_H
#define MAGIC3D_SPHERE_H

#include <Mesh/TriangleMesh.h>
#include <CollisionShapes\CollisionShape.h>
#include <CollisionShapes\SphereCollisionShape.h>
#include <Mesh\TriangleMeshBuilder.h>
#include <Geometry\Geometry.h>

namespace Magic3D
{


class Sphere : public Geometry
{
    Vector3 center;
    Scalar radius;

    unsigned int fidelity;

    mutable std::shared_ptr<CollisionShape> collisionShape;
    mutable std::shared_ptr<TriangleMesh> triangleMesh;

public:
    inline Sphere(Scalar radius, const Vector3& center = Vector3(0,0,0), unsigned int fidelity = 55) :
        center(center), radius(radius), fidelity(fidelity) {}

    virtual const CollisionShape& getCollisionShape() const
    {
        if (collisionShape == nullptr)
            collisionShape = std::make_shared<SphereCollisionShape>(radius, center);
        return *this->collisionShape;
    }

    virtual const TriangleMesh& getTriangleMesh() const
    {
        throw_MagicException("Triangle mesh generation from sphere is not currently supported");
    }

};

};


#endif