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
    Scalar radius;
    Matrix4 transform;

    unsigned int fidelity;

    mutable std::shared_ptr<CollisionShape> collisionShape;
    mutable std::shared_ptr<TriangleMesh> triangleMesh;

    inline void markDirty()
    {
        this->collisionShape = nullptr;
        this->triangleMesh = nullptr;
    }

public:
    inline Sphere(Scalar radius,unsigned int fidelity = 3) :
        radius(radius), fidelity(fidelity) {}

    inline Scalar getRadius() const
    {
        return radius;
    }

    inline Vector3 getTranslation() const
    {
        auto data = transform.getArray();
        return Vector3(
            data[12],
            data[13],
            data[14]
        );
    }

    virtual void positionTransform(const Matrix4& matrix)
    {
        this->transform.multiply(matrix);
        this->markDirty();
    }

    virtual const CollisionShape& getCollisionShape() const
    {
        // TODO: apply arbitary transform to collision shape
        if (collisionShape == nullptr)
            collisionShape = std::make_shared<SphereCollisionShape>(radius);
        return *this->collisionShape;
    }

    virtual const TriangleMesh& getTriangleMesh() const;

};

};


#endif