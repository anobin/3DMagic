#ifndef MAGIC3D_COLLISION_SHAPE_H
#define MAGIC3D_COLLISION_SHAPE_H


#include <Math/Math.h>

// include bullet physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <memory>


namespace Magic3D
{
    
class Sphere;
class Box;

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
    mutable std::shared_ptr<Box> aabb; // axis-aligned bounding box

    Matrix3 rotation;
    Vector3 translation;

public:
    inline CollisionShape(std::shared_ptr<btCollisionShape> shape, 
        Matrix3 rotation = Matrix3(), Vector3 translation = Vector3()) 
    : shape(shape), rotation(rotation), translation(translation) {}
    
    inline btCollisionShape& _getShape() const
    {
        return *this->shape;
    }

    inline const Matrix3& _getRotation() const
    {
        return this->rotation;
    }

    inline const Vector3& _getTranslation() const
    {
        return this->translation;
    }

    /// Get bounding sphere of collision shape.
    const Sphere& getBoundingSphere() const;

    const Box& getAABB() const;
};



};




#endif




