#ifndef MAGIC3D_COMPOUND_GEOMETRY_H
#define MAGIC3D_COMPOUND_GEOMETRY_H


#include <vector>
#include <memory>

#include <Geometry\Geometry.h>
#include <Exceptions\MagicException.h>

namespace Magic3D
{

class CompoundGeometry : public Geometry
{
    const std::vector<std::shared_ptr<Geometry>> list;

    mutable std::shared_ptr<CollisionShape> shape;
    mutable std::shared_ptr<TriangleMesh> mesh;
    mutable std::shared_ptr<Sphere> sphere;
public:
    inline CompoundGeometry(const std::vector<std::shared_ptr<Geometry>>& list) :
        list(list) {}

    virtual void positionTransform(const Transform& transform)
    {
        throw_MagicException("Arbitary position transforms on compound geometry not currently supported");
    }

    virtual const CollisionShape& getCollisionShape() const;

    virtual const TriangleMesh& getTriangleMesh() const;

    virtual const Sphere& getBoundingSphere() const;
};

};


#endif