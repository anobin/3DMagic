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
/** Implementation file for Point3 Generic implementation
 *
 * @file Point3.cc
 * @author Andrew Keating
 */

#include <Math/Intel/Matrix4.h>
#include <Math/Intel/Matrix3.h>
#include <Math/Intel/Point3.h>




/// transform this point using a matrix
void Point3::transform(const Matrix4 &m)
{
    Scalar tmp[3];
    tmp[0] = m.get(0,0) * x + m.get(1,0) * y + m.get(2,0) * z + m.get(3,0); 
    tmp[1] = m.get(0,1) * x + m.get(1,1) * y + m.get(2,1) * z + m.get(3,1); 
    tmp[2] = m.get(0,2) * x + m.get(1,2) * y + m.get(2,2) * z + m.get(3,2);
    
    x = tmp[0];
    y = tmp[1];
    z = tmp[2];
}

/// transform a point using a matrix and store the results in this point
void Point3::transform(const Point3 &p, const Matrix4 &m)
{
    x = m.get(0,0) * p.getX() + m.get(1,0) * p.getY() + m.get(2,0) *  p.getZ() + m.get(3,0); 
    y = m.get(0,1) * p.getX() + m.get(1,1) * p.getY() + m.get(2,1) *  p.getZ() + m.get(3,1);  
    z = m.get(0,2) * p.getX() + m.get(1,2) * p.getY() + m.get(2,2) * p.getZ() + m.get(3,2);
}

/// rotate this point using a matrix
void Point3::rotate(const Matrix3 &m)
{
    Scalar tmp[3];
    tmp[0] = m.get(0,0) * x + m.get(1,0) * y + m.get(2,0) * z;   
    tmp[1] = m.get(0,1) * x + m.get(1,1) * y + m.get(2,1) * z;   
    tmp[2] = m.get(0,2) * x + m.get(1,2) * y + m.get(2,2) * z;
    
    x = tmp[0];
    y = tmp[1];
    z = tmp[2];
}

/// rotate a point using a matrix and store the result in this point
void Point3::rotate(const Point3 &p, const Matrix3 &m)
{
    x = m.get(0,0) * p.getX() + m.get(1,0) * p.getY() + m.get(2,0) * p.getZ();    
    y = m.get(0,1) * p.getX() + m.get(1,1) * p.getY() + m.get(2,1) * p.getZ();    
    z = m.get(0,2) * p.getX() + m.get(1,2) * p.getY() + m.get(2,2) * p.getZ();
}







































