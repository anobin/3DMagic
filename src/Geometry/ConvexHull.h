#ifndef MAGIC3D_CONVEX_HULL_H
#define MAGIC3D_CONVEX_HULL_H

#include <Geometry\Geometry.h>
#include <CollisionShapes\CollisionShape.h>
#include <Mesh\TriangleMesh.h>

#include <BulletCollision\CollisionShapes\btShapeHull.h>

namespace Magic3D
{

class ConvexHull : public Geometry
{
    std::shared_ptr<btShapeHull> hull;
    std::shared_ptr<btConvexHullShape> internalShape;

    mutable std::shared_ptr<CollisionShape> shape;
    mutable std::shared_ptr<TriangleMesh> mesh;

public:
    ConvexHull(const Geometry& exactGeometry, Scalar margin = 0.04f);

    virtual void positionTransform(const Transform& transform)
    {
        throw_MagicException("Transforming a convex hull is not currently supported");
    }

    virtual const CollisionShape& getCollisionShape() const
    {
        if (this->shape != nullptr)
            return *this->shape;

        this->shape = std::make_shared<CollisionShape>(this->internalShape);
        return *this->shape;
    }

    virtual const TriangleMesh& getTriangleMesh() const;

};

};


#endif