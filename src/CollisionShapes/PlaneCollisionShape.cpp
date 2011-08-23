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
/** Implementation file for PlaneCollisionShape base class
 *
 * @file PlaneCollisionShape.cpp
 * @author Andrew Keating
 */
 
#include <CollisionShapes/PlaneCollisionShape.h>


namespace Magic3D
{
    
/// destructor
PlaneCollisionShape::~PlaneCollisionShape()
{
    // intentionally left blank
}   
    
    
/// get the bullet physics collison shape
btCollisionShape* PlaneCollisionShape::getShape()
{
    return &this->shape;
}      
    
    
    
    
    
};










