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
/** Implementation file for Position Generic implementation
 *
 * @file Position.cc
 * @author Andrew Keating
 */

#include <Math/Generic/Position.h>


/// get a transformation matrix that represents this position
void Position::getTransformMatrix(Matrix4& out)
{
    // get local x axis
    Vector3 xAxis;
    xAxis.crossProduct(up, forward);

    // fill in x axis, first column
    out.set(0,0,xAxis.getX());
    out.set(1,0,xAxis.getY());
    out.set(2,0,xAxis.getZ());
    out.set(3,0,0.0f);
    
    // fill in y axis, second column
    out.set(4,0,up.getX());
    out.set(5,0,up.getY());
    out.set(6,0,up.getZ());
    out.set(7,0,0.0f);       
                            
    // fill in z axis, thrid column
    out.set(8,0,forward.getX());
    out.set(9,0,forward.getY());
    out.set(10,0,forward.getZ());
    out.set(11,0,0.0f);

    // fill in translation, fourth column
    out.set(12,0,location.getX());
    out.set(13,0,location.getY());
    out.set(14,0,location.getZ());
    out.set(15,0,1.0f); // W component is always 1, duh..
}

/// get a rotation matrix that represents the orientation of this position
void Position::getRotationMatrix(Matrix3& out)
{
    // get local x axis
    Vector3 xAxis;
    xAxis.crossProduct(up, forward);

    // fill in x axis, first column
    out.set(0,0,xAxis.getX());
    out.set(1,0,xAxis.getY());
    out.set(2,0,xAxis.getZ());
    
    // fill in y axis, second column
    out.set(3,0,up.getX());
    out.set(4,0,up.getY());
    out.set(5,0,up.getZ());
                            
    // fill in z axis, thrid column
    out.set(6,0,forward.getX());
    out.set(7,0,forward.getY());
    out.set(8,0,forward.getZ());
}

/// get a matrix that can be used to move objects around the camera
void Position::getCameraMatrix(Matrix4& out)
{
    // z is reversed
    Vector3 z(-forward.getX(), -forward.getY(), -forward.getZ());

    // get x vector
    Vector3 x;
    x.crossProduct(up, z);

    // matrix is transposed (rows instead of columns)
    #define MAGIC3D_A(row,col,value)  out.set(col*4+row,0,value)
        MAGIC3D_A(0, 0, x.getX());
        MAGIC3D_A(0, 1, x.getY());
        MAGIC3D_A(0, 2 ,x.getZ());
        MAGIC3D_A(0, 3, 0.0);
        MAGIC3D_A(1, 0, up.getX());
        MAGIC3D_A(1, 1, up.getY());
        MAGIC3D_A(1, 2, up.getZ());
        MAGIC3D_A(1, 3, 0.0);
        MAGIC3D_A(2, 0, z.getX());
        MAGIC3D_A(2, 1, z.getY());
        MAGIC3D_A(2, 2, z.getZ());
        MAGIC3D_A(2, 3, 0.0);
        MAGIC3D_A(3, 0, 0.0);
        MAGIC3D_A(3, 1, 0.0);
        MAGIC3D_A(3, 2, 0.0);
        MAGIC3D_A(3, 3, 1.0);
    #undef MAGIC3D_A
        
    // apply reversed translation
    Matrix4 trans;
    trans.createTranslationMatrix(-location.getX(), -location.getY(), -location.getZ());
    out.multiply(trans);
}

/// rotate this position in world coordinates
void Position::rotate(Scalar angle, const Vector3 &axis)
{
    // create rotation matrix
    Matrix4 rotMat;
    rotMat.createRotationMatrix(angle, axis.getX(), axis.getY(), axis.getZ());

    // transform the up vector
    up.transform(rotMat);

    // transform the forward vector
    forward.transform(rotMat);
}

/// rotate this position in local coordinates
void Position::rotateLocal(Scalar angle, const Vector3 &axis)
{
    // transform the rotation axis to world coordinates
    Matrix3 rotMat;
    this->getRotationMatrix(rotMat);
    Vector3 worldAxis(axis);
    worldAxis.rotate(rotMat);
    
    // perform rotation
    this->rotate(angle, worldAxis);
}

/// normalize this position
void Position::normalize()
{
    // we normalize in reference to the forward vector
    
    // get x axis
    Vector3 cross;
    cross.crossProduct(up, forward);
    
    // recalculate up vector
    up.crossProduct(cross, forward);
    
    // normalize both vectors too
    up.normalize();
    forward.normalize();
}


























