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
/** Header file for PlaneCollisionShape class
 *
 * @file PlaneCollisionShape.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_PLANE_COLLISION_SHAPE_H
#define MAGIC3D_PLANE_COLLISION_SHAPE_H


#include "CollisionShape.h"

// include bullet physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>


namespace Magic3D
{

/** One-sided Infinite plane. Usually used to bound physics environment area.
 */
class PlaneCollisionShape : public CollisionShape
{
protected:
    btStaticPlaneShape shape;
    
    /// get the bullet physics collison shape
    virtual btCollisionShape* getShape();

public:
    /// default constructor
    inline PlaneCollisionShape(const Vector3& normal ): 
        shape( btVector3(normal.x(), normal.y(), normal.z()), 0 ) {}
    
    /// destructor
    virtual ~PlaneCollisionShape();



};




};






#endif




