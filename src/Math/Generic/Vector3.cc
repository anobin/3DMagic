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
/** Implementation file for the Generic Vector3 Implementation
 *
 * @file Vector3.cc
 * @author Andrew Keating
 */ 

#include <Math/Generic/Vector3.h>

#include <Math/Generic/Matrix4.h>
#include <Math/Generic/Matrix3.h>



/// transform this vector using a matrix
void Vector3::transform(const Matrix4 &m)
{
    Scalar tmp[3];
    tmp[0] = m.get(0,0) * x + m.get(4,0) * y + m.get(8,0) *  z + m.get(12,0); 
    tmp[1] = m.get(1,0) * x + m.get(5,0) * y + m.get(9,0) *  z + m.get(13,0); 
    tmp[2] = m.get(2,0) * x + m.get(6,0) * y + m.get(10,0) * z + m.get(14,0);
    
    x = tmp[0];
    y = tmp[1];
    z = tmp[2];
}

/// transform another vector using a matrix and store the result in this vector
void Vector3::transform(const Vector3 &v, const Matrix4 &m)
{
    x = m.get(0,0) * v.getX() + m.get(4,0) * v.getY() + m.get(8,0) *  v.getZ() + m.get(12,0); 
    y = m.get(1,0) * v.getX() + m.get(5,0) * v.getY() + m.get(9,0) *  v.getZ() + m.get(13,0);  
    z = m.get(2,0) * v.getX() + m.get(6,0) * v.getY() + m.get(10,0) * v.getZ() + m.get(14,0);
}

/// rotate this vector using a matrix
void Vector3::rotate(const Matrix3 &m)
{
    Scalar tmp[3];
    tmp[0] = m.get(0,0) * x + m.get(3,0) * y + m.get(6,0) * z;   
    tmp[1] = m.get(1,0) * x + m.get(4,0) * y + m.get(7,0) * z;   
    tmp[2] = m.get(2,0) * x + m.get(5,0) * y + m.get(8,0) * z;
    
    x = tmp[0];
    y = tmp[1];
    z = tmp[2];
}

/// rotate another vector using a matrix and store the result in this vector
void Vector3::rotate(const Vector3 &v, const Matrix3 &m)
{
    x = m.get(0,0) * v.getX() + m.get(3,0) * v.getY() + m.get(6,0) * v.getZ();    
    y = m.get(1,0) * v.getX() + m.get(4,0) * v.getY() + m.get(7,0) * v.getZ();    
    z = m.get(2,0) * v.getX() + m.get(5,0) * v.getY() + m.get(8,0) * v.getZ();
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    