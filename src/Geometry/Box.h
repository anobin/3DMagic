#ifndef MAGIC3D_BOX_H
#define MAGIC3D_BOX_H

#include <Mesh/TriangleMesh.h>
#include <CollisionShapes\CollisionShape.h>
#include <Mesh\TriangleMeshBuilder.h>
#include <Geometry\Geometry.h>

namespace Magic3D
{


class Box : public Geometry
{
    Scalar width, height, depth;
    Matrix4 transform;
    bool transformApplied;

    mutable std::shared_ptr<CollisionShape> collisionShape;
    mutable std::shared_ptr<TriangleMesh> triangleMesh;

    inline void markDirty()
    {
        this->collisionShape = nullptr;
        this->triangleMesh = nullptr;
    }

public:
    inline Box(Scalar width, Scalar height, Scalar depth) :
        width(width), height(height), depth(depth), transformApplied(false){}

    virtual void positionTransform(const Matrix4& matrix)
    {
        this->transform.multiply(matrix);
        this->transformApplied = true;
        this->markDirty();
    }

    virtual const CollisionShape& getCollisionShape() const;

    virtual const TriangleMesh& getTriangleMesh() const;

    virtual const Sphere& getBoundingSphere() const
    {
        // TODO: stop using triangle mesh for bounding sphere
        return this->getTriangleMesh().getBoundingSphere();
    }

};

};


#endif