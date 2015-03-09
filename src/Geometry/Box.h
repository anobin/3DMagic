#ifndef MAGIC3D_BOX_H
#define MAGIC3D_BOX_H

#include <Mesh/TriangleMesh.h>
#include <CollisionShapes\CollisionShape.h>
#include <CollisionShapes\BoxCollisionShape.h>
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

    virtual const CollisionShape& getCollisionShape() const
    {
        // TODO: apply transform to collision shape; can try to use Bullet's project() for rotation and translation
        //       and fall back to full shape regeneration for scaling
        if (this->transformApplied)
            throw_MagicException("Arbitary transforms on box geometry that is used as a collision shape is not currently supported");

        if (collisionShape == nullptr)
            collisionShape = std::make_shared<BoxCollisionShape>(width, height, depth);
        return *this->collisionShape;
    }

    virtual const TriangleMesh& getTriangleMesh() const;

};

};


#endif