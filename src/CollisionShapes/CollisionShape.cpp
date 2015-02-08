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
 
#include <CollisionShapes/CollisionShape.h>
#include <CollisionShapes\SphereCollisionShape.h>

namespace Magic3D
{
    
/// destructor
CollisionShape::~CollisionShape()
{
    // intentionally left blank
}   
    
    
std::shared_ptr<SphereCollisionShape> CollisionShape::getBoundingSphere()
{
    btVector3 bt_center;
    btScalar bt_radius;
    this->getShape()->getBoundingSphere(bt_center, bt_radius);
    
    return std::make_shared<SphereCollisionShape>((Scalar)bt_radius,
        Point3(bt_center.getX(), bt_center.getY(), bt_center.getZ()));
}
    
    
    
    
    
    
    
};










