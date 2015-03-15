#ifndef MAGIC3D_BOUNDED_PLANE_H
#define MAGIC3D_BOUNDED_PLANE_H

#include <Mesh/TriangleMesh.h>
#include <CollisionShapes\CollisionShape.h>
#include <Mesh\TriangleMeshBuilder.h>
#include <Geometry\Geometry.h>
#include <Util\Units.h>

namespace Magic3D
{


class BoundedPlane : public Geometry
{
    Scalar width, height;
    unsigned int slices, stacks;
    Scalar texPerX, texPerY;
    Matrix4 transform;

    mutable std::shared_ptr<CollisionShape> collisionShape;
    mutable std::shared_ptr<TriangleMesh> triangleMesh;

    inline void markDirty()
    {
        this->collisionShape = nullptr;
        this->triangleMesh = nullptr;
    }

public:
    inline BoundedPlane(
        Scalar width, 
        Scalar height, 
        unsigned int slices = 1, 
        unsigned int stacks = 1,
        Scalar texPerX = 1,
        Scalar texPerY = 1
    ) : width(width), height(height), slices(slices), stacks(stacks), 
    texPerX(texPerX), texPerY(texPerY)
    {}

    virtual void positionTransform(const Transform& transform)
    {
        Matrix4 tmp; transform.getCombinedMatrix(tmp);

        this->transform.multiply(tmp);
        this->markDirty();
    }

    virtual const CollisionShape& getCollisionShape() const
    {
        throw_MagicException("Using a bounded plane as a collision shape is not currently supported");
    }

    virtual const TriangleMesh& getTriangleMesh() const;

    virtual const Sphere& getBoundingSphere() const
    {
        // TODO: stop using triangle mesh for bounding sphere
        return this->getTriangleMesh().getBoundingSphere();
    }

    virtual const Box& getAABB() const
    {
        return this->getTriangleMesh().getAABB();
    }
};

};


#endif