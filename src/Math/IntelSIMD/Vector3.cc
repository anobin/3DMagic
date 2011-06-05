/* 
Copyright (c) 2011 Matthew Del Buono
Based on the Generic Vector3 implementation by Andrew Keating

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
/** Implementation file for the Intel SIMD Vector3 Implementation
 *
 * @file Vector3.cc
 * @author Matthew Del Buono
 */ 

#include <Math/Generic/Vector3.h>
#include <Math/Generic/Matrix4.h>
#include <Math/Generic/Matrix3.h>



/// transform this vector using a matrix
void Vector3::transform(const Matrix4 &m)
{
    Scalar tmp[3];
    tmp[0] = m.get(0,0) * x + m.get(1,0) * y + m.get(2,0) * z + m.get(3,0); 
    tmp[1] = m.get(0,1) * x + m.get(1,1) * y + m.get(2,1) * z + m.get(3,1); 
    tmp[2] = m.get(0,2) * x + m.get(1,2) * y + m.get(2,2) * z + m.get(3,2);
    
    x = tmp[0];
    y = tmp[1];
    z = tmp[2];
}

/// transform another vector using a matrix and store the result in this vector
void Vector3::transform(const Vector3 &v, const Matrix4 &m)
{
    x = m.get(0,0) * v.getX() + m.get(1,0) * v.getY() + m.get(2,0) *  v.getZ() + m.get(3,0); 
    y = m.get(0,1) * v.getX() + m.get(1,1) * v.getY() + m.get(2,1) *  v.getZ() + m.get(3,1);  
    z = m.get(0,2) * v.getX() + m.get(1,2) * v.getY() + m.get(2,2) * v.getZ() + m.get(3,2);
}

/// rotate this vector using a matrix
void Vector3::rotate(const Matrix3 &m)
{
    Scalar tmp[3];
    tmp[0] = m.get(0,0) * x + m.get(1,0) * y + m.get(2,0) * z;   
    tmp[1] = m.get(0,1) * x + m.get(1,1) * y + m.get(2,1) * z;   
    tmp[2] = m.get(0,2) * x + m.get(1,2) * y + m.get(2,2) * z;
    
    x = tmp[0];
    y = tmp[1];
    z = tmp[2];
}

/// rotate another vector using a matrix and store the result in this vector
void Vector3::rotate(const Vector3 &v, const Matrix3 &m)
{
    x = m.get(0,0) * v.getX() + m.get(1,0) * v.getY() + m.get(2,0) * v.getZ();    
    y = m.get(0,1) * v.getX() + m.get(1,1) * v.getY() + m.get(2,1) * v.getZ();    
    z = m.get(0,2) * v.getX() + m.get(1,2) * v.getY() + m.get(2,2) * v.getZ();
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    