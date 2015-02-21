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
/** Header file for SphereCollisionShape class
 *
 * @file SphereCollisionShape.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_SPHERE_COLLISION_SHAPE_H
#define MAGIC3D_SPHERE_COLLISION_SHAPE_H


#include "CollisionShape.h"

// include bullet physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>


namespace Magic3D
{

/** Perfect sphere collision shape.
 */
class SphereCollisionShape : public CollisionShape
{
protected:
    btSphereShape shape;

    Scalar radius;
    
    /// get the bullet physics collison shape
    virtual btCollisionShape* getShape();

public:
    /// default constructor
    inline SphereCollisionShape(Scalar radius, Vector3 offset = Vector3()): CollisionShape(offset), 
		radius(radius), shape((btScalar)radius) {}
    
    /// destructor
    virtual ~SphereCollisionShape();

    inline const Scalar& getRadius() const
    {
        return this->radius;
    }

};




};






#endif




