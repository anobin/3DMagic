#ifndef MAGIC3D_COLLISION_SHAPE_H
#define MAGIC3D_COLLISION_SHAPE_H


#include <Math/Vector.h>

// include bullet physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <memory>


namespace Magic3D
{
    
class Sphere;

/** Base class for all collision shapes used in physics collison calculations.
 * Note that the physical shape of an object can (and probably should) not match
 * the graphical meshes exactly; as such bounding volumes of collision shapes
 * should not be used graphical operations such as culling and intersection
 * detection.
 */
class CollisionShape
{
protected: 
    std::shared_ptr<btCollisionShape> shape;
    mutable std::shared_ptr<Sphere> boundingSphere;

public:
    inline CollisionShape(std::shared_ptr<btCollisionShape> shape) : shape(shape) {}
    
    inline btCollisionShape& _getShape() const
    {
        return *this->shape;
    }

    /// Get bounding sphere of collision shape.
    const Sphere& getBoundingSphere() const;

};



};




#endif




