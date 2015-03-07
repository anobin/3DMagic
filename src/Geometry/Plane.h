#ifndef MAGIC3D_PLANE_H
#define MAGIC3D_PLANE_H

#include <Mesh/TriangleMesh.h>
#include <CollisionShapes\CollisionShape.h>
#include <CollisionShapes\PlaneCollisionShape.h>
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

    virtual const CollisionShape& getCollisionShape() const
    {
        if (collisionShape == nullptr)
            collisionShape = std::make_shared<PlaneCollisionShape>(normal, distance);
        return *this->collisionShape;
    }

    virtual const TriangleMesh& getTriangleMesh() const
    {
        if (triangleMesh == nullptr)
        {
            // can't really render a infinite plane, so just do a really big, low poly one
            triangleMesh = TriangleMeshBuilder::buildFlatSurface(
                10000 * METER, 10000 * METER, 1, 1, false, 0, 0);
            if (distance != 0.0f)
            {
                Matrix4 matrix;
                matrix.createTranslationMatrix(normal.x()*distance, normal.y()*distance, 
                    normal.z()*distance);
                triangleMesh->positionTransform(matrix);
            }
        }

        return *this->triangleMesh;
    }

};

};


#endif