/* 
Copyright (c) 2011 Andrew Keating

This file is part of 3DMagic.

3DMagic is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

3DMagic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with 3DMagic.  If not, see <http://www.gnu.org/licenses/>.

*/
/** Header file for CollisionShape base class
 *
 * @file CollisionShape.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_COLLISION_SHAPE_H
#define MAGIC3D_COLLISION_SHAPE_H


#include <Math/Vector.h>

// include bullet physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <memory>


namespace Magic3D
{
    
class Object;
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
    friend class Object;
    
    /// get the bullet physics collison shape
    virtual btCollisionShape* getShape() = 0;

	Vector3 offset;

	/// constructor specifying an offset in local space, defaults to origin
	inline CollisionShape(Vector3 offset = Vector3()): offset(offset) {}

public:
    
    /// destructor
    virtual ~CollisionShape();

    inline const Vector3& getOffset() const
    {
        return this->offset;
    }
    
    /// Get bounding sphere of collision shape.
    std::shared_ptr<Sphere> getBoundingSphere() const;

};




};






#endif




