#ifndef MAGIC3D_GEOMETRY_H
#define MAGIC3D_GEOMETRY_H


namespace Magic3D
{
    
class TriangleMesh;
class CollisionShape;

class Geometry
{
public:
    virtual const CollisionShape& getCollisionShape() const = 0;

    virtual const TriangleMesh& getTriangleMesh() const = 0;

};

};


#endif