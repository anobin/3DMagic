#ifndef MAGIC3D_PLANE_H
#define MAGIC3D_PLANE_H

#include <Mesh/TriangleMesh.h>
#include <CollisionShapes\CollisionShape.h>
#include <Mesh\TriangleMeshBuilder.h>
#include <Geometry\Geometry.h>
#include <Util\Units.h>

namespace Magic3D
{


class Plane : public Geometry
{
    Vector3 normal;
    Scalar distance;

    mutable std::shared_ptr<CollisionShape> collisionShape;
    mutable std::shared_ptr<TriangleMesh> triangleMesh;

public:
    inline Plane(const Vector3& normal, Scalar distance = 0.0f) :
        normal(normal), distance(distance) {}

    virtual void positionTransform(const Transform& transform)
    {
        throw_MagicException("Arbitary position transforms on infinite plane geometry are not currently supported");
    }

    virtual const CollisionShape& getCollisionShape() const
    {
        if (collisionShape != nullptr)
            return *this->collisionShape;

        collisionShape = std::make_shared<CollisionShape>(
            std::make_shared<btStaticPlaneShape>(btVector3(normal.x(), normal.y(), normal.z()), distance)
        );
        return *this->collisionShape;
    }

    virtual const TriangleMesh& getTriangleMesh() const
    {
        throw_MagicException("Using a infinite plane as a graphical mesh is not currently supported");
    }

};

};


#endif