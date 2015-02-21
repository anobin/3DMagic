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
void Position::getTransformMatrix(Matrix4& out) const
{
    // get local x axis
    Vector3 xAxis = up * forward;

    // fill in x axis, first column
    out.set(0,0,xAxis.x());
    out.set(0,1,xAxis.y());
    out.set(0,2,xAxis.z());
    out.set(0,3,0.0f);
    
    // fill in y axis, second column
    out.set(1,0,up.x());
    out.set(1,1,up.y());
    out.set(1,2,up.z());
    out.set(1,3,0.0f);       
                            
    // fill in z axis, thrid column
    out.set(2,0,forward.x());
    out.set(2,1,forward.y());
    out.set(2,2,forward.z());
    out.set(2,3,0.0f);

    // fill in translation, fourth column
    out.set(3,0,location.x());
    out.set(3,1,location.y());
    out.set(3,2,location.z());
    out.set(3,3,1.0f); // W component is always 1, duh..
}

/// get a rotation matrix that represents the orientation of this position
void Position::getRotationMatrix(Matrix3& out) const
{
    // get local x axis
    Vector3 xAxis = up * forward;

    // fill in x axis, first column
    out.set(0,0,xAxis.x());
    out.set(0,1,xAxis.y());
    out.set(0,2,xAxis.z());
    
    // fill in y axis, second column
    out.set(1,0,up.x());
    out.set(1,1,up.y());
    out.set(1,2,up.z());
                            
    // fill in z axis, thrid column
    out.set(2,0,forward.x());
    out.set(2,1,forward.y());
    out.set(2,2,forward.z());
}

/// get a matrix that can be used to move objects around the camera
void Position::getCameraMatrix(Matrix4& out) const
{
    // z is reversed
    Vector3 z(-forward.x(), -forward.y(), -forward.z());

    // get x vector
    Vector3 x = up * z;

    // matrix is transposed (rows instead of columns)
    #define MAGIC3D_A(row,col,value)  out.set(col,row,value)
        MAGIC3D_A(0, 0, x.x());
        MAGIC3D_A(0, 1, x.y());
        MAGIC3D_A(0, 2 ,x.z());
        MAGIC3D_A(0, 3, 0.0);
        MAGIC3D_A(1, 0, up.x());
        MAGIC3D_A(1, 1, up.y());
        MAGIC3D_A(1, 2, up.z());
        MAGIC3D_A(1, 3, 0.0);
        MAGIC3D_A(2, 0, z.x());
        MAGIC3D_A(2, 1, z.y());
        MAGIC3D_A(2, 2, z.z());
        MAGIC3D_A(2, 3, 0.0);
        MAGIC3D_A(3, 0, 0.0);
        MAGIC3D_A(3, 1, 0.0);
        MAGIC3D_A(3, 2, 0.0);
        MAGIC3D_A(3, 3, 1.0);
    #undef MAGIC3D_A
        
    // apply reversed translation
    Matrix4 trans;
    trans.createTranslationMatrix(-location.x(), -location.y(), -location.z());
    out.multiply(trans);
}

/// rotate this position in world coordinates
void Position::rotate(Scalar angle, const Vector3 &axis)
{
    // create rotation matrix
    Matrix4 rotMat;
    rotMat.createRotationMatrix(angle, axis.x(), axis.y(), axis.z());

    // transform the up vector
    up = up.transform(rotMat);

    // transform the forward vector
    forward = forward.transform(rotMat);
}

/// rotate this position in local coordinates
void Position::rotateLocal(Scalar angle, const Vector3 &axis)
{
    // transform the rotation axis to world coordinates
    Matrix3 rotMat;
    this->getRotationMatrix(rotMat);
    Vector3 worldAxis = axis.rotate(rotMat);
    
    // perform rotation
    this->rotate(angle, worldAxis);
}

/// normalize this position
void Position::normalize()
{
    // we normalize in reference to the forward vector
    
    // get x axis
    Vector3 cross = forward * up;
    
    // recalculate up vector
    up = cross * forward;
    
    // normalize both vectors too
    up = up.normalize();
    forward = forward.normalize();
}


























