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


#include "../Math/Vector.h"

// include bullet physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>


namespace Magic3D
{
    
class PhysicalBody;

/** Base class for all collision shapes used in physics collison calculations.
 * Note that the physical shape of an object can (and probably should) not match
 * the graphical meshes exactly; as such bounding volumes of collision shapes
 * should not be used graphical operations such as culling and intersection
 * detection.
 */
class CollisionShape
{
protected:
    friend class PhysicalBody;
    
    /// get the bullet physics collison shape
    virtual btCollisionShape* getShape() = 0;

    /// default constructor
    inline CollisionShape() {}

public:
    
    /// destructor
    virtual ~CollisionShape();
    
    /** Get bounding sphere of collision shape. Does not necessarily (and probably wouldn't)
     * represent the bounding sphere of the graphical meshes since they can be different
     * from the physical collision shape (common for efficiency reasons).
     */
     inline Vector3 getBoundingSphere(Scalar* radius)
     {
         btVector3 bt_center;
         btScalar bt_radius;
         this->getShape()->getBoundingSphere(bt_center, bt_radius);
		 (*radius) = (Scalar)bt_radius;
		 return Vector3(
			bt_center.getX(),
			bt_center.getY(),
			bt_center.getZ()
		);
     }




};




};






#endif




