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
    Vector3 dimensions;
    Geometry::Transform transform;

    mutable std::shared_ptr<CollisionShape> collisionShape;
    mutable std::shared_ptr<TriangleMesh> triangleMesh;

    inline void markDirty()
    {
        this->collisionShape = nullptr;
        this->triangleMesh = nullptr;
    }

public:
    inline Box(Scalar width, Scalar height, Scalar depth) :
        dimensions(width, height, depth) {}

    inline const Vector3& getDimensions()
    {
        return this->dimensions;
    }

    virtual void positionTransform(const Geometry::Transform& transform)
    {
        // copy over rotatation and translation to use later when 
        // generating mesh and shape
        this->transform.rotation.multiply(transform.rotation);
        this->transform.translation += transform.translation;

        // apply scale directly to dimensions, shape and mesh will be rebuilt from these
        this->dimensions[0] *= transform.scale[0];
        this->dimensions[1] *= transform.scale[1];
        this->dimensions[2] *= transform.scale[2];

        this->markDirty();
    }

    virtual const CollisionShape& getCollisionShape() const;

    virtual const TriangleMesh& getTriangleMesh() const;

};

};


#endif