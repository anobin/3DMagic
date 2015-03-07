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
    Vector3 center;
    Scalar width, height, depth;

    mutable std::shared_ptr<CollisionShape> collisionShape;
    mutable std::shared_ptr<TriangleMesh> triangleMesh;

public:
    inline Box(Scalar width, Scalar height, Scalar depth,
        const Vector3& center = Vector3(0, 0, 0)) :
        width(width), height(height), depth(depth), center(center) {}

    virtual const CollisionShape& getCollisionShape() const
    {
        if (collisionShape == nullptr)
            collisionShape = std::make_shared<BoxCollisionShape>(width, height, depth);
        return *this->collisionShape;
    }

    virtual const TriangleMesh& getTriangleMesh() const
    {
        if (triangleMesh == nullptr)
        {
            triangleMesh = TriangleMeshBuilder::buildBox(width, height, depth);
            if (!center.isAtOrigin())
            {
                Matrix4 matrix;
                matrix.createTranslationMatrix(center.x(), center.y(), center.z());
                triangleMesh->positionTransform(matrix);
            }
        }

        return *this->triangleMesh;
    }

};

};


#endif