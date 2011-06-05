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
/** Implementation file for Matrix3 Generic implementation
 *
 * @file Matrix3.cc
 * @author Andrew Keating
 */ 

#include <Math/Intel/Matrix3.h>

/// the identity matrix
const Scalar Matrix3::identity[] = {1.0f, 0.0f, 0.0f, 
                                    0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f};

                                    
/// multiply this matrix and another
void Matrix3::multiply(const Matrix3 &m)
{
#define MAGIC3D_A(row,col)  data[(col*3)+row]
#define MAGIC3D_B(row,col)  m.data[(col*3)+row]
#define MAGIC3D_P(row,col)  tmp[(col*3)+row]

        Scalar tmp[3*3];
        
        // The math is strong with this one
        // grab a math book if you want to understand this crap
        for (int i = 0; i < 3; i++) 
        {
            Scalar ai0=MAGIC3D_A(i,0),  ai1=MAGIC3D_A(i,1),  ai2=MAGIC3D_A(i,2);
            MAGIC3D_P(i,0) = ai0 * MAGIC3D_B(0,0) + ai1 * MAGIC3D_B(1,0) + ai2 * MAGIC3D_B(2,0);
            MAGIC3D_P(i,1) = ai0 * MAGIC3D_B(0,1) + ai1 * MAGIC3D_B(1,1) + ai2 * MAGIC3D_B(2,1);
            MAGIC3D_P(i,2) = ai0 * MAGIC3D_B(0,2) + ai1 * MAGIC3D_B(1,2) + ai2 * MAGIC3D_B(2,2);
        }
        
        // copy data back in to make *= operation
        memcpy(data, tmp, sizeof(Scalar)*3*3);
        
#undef MAGIC3D_A
#undef MAGIC3D_B
#undef MAGIC3D_P
}

/// multiply two matrixes and store the result in this matrix
void Matrix3::multiply(const Matrix3 &m1, const Matrix3 &m2)
{
#define MAGIC3D_A(row,col)  m1.data[(col*3)+row]
#define MAGIC3D_B(row,col)  m2.data[(col*3)+row]
#define MAGIC3D_P(row,col)  data[(col*3)+row]
        
        // The math is strong with this one
        // grab a math book if you want to understand this crap
        for (int i = 0; i < 3; i++) 
        {
            Scalar ai0=MAGIC3D_A(i,0),  ai1=MAGIC3D_A(i,1),  ai2=MAGIC3D_A(i,2);
            MAGIC3D_P(i,0) = ai0 * MAGIC3D_B(0,0) + ai1 * MAGIC3D_B(1,0) + ai2 * MAGIC3D_B(2,0);
            MAGIC3D_P(i,1) = ai0 * MAGIC3D_B(0,1) + ai1 * MAGIC3D_B(1,1) + ai2 * MAGIC3D_B(2,1);
            MAGIC3D_P(i,2) = ai0 * MAGIC3D_B(0,2) + ai1 * MAGIC3D_B(1,2) + ai2 * MAGIC3D_B(2,2);
        }
        
#undef MAGIC3D_A
#undef MAGIC3D_B
#undef MAGIC3D_P
}

/// turn this matrix into a scale matrix
void Matrix3::createScaleMatrix(Scalar x, Scalar y, Scalar z)
{
    // load identity matrix
    memcpy(this->data, Matrix3::identity, sizeof(Scalar)*3*3);
    
    // apply scaling
    data[0] = x; 
    data[4] = y; 
    data[8] = z;
}

/// turn this matrix into a rotation matrix
void Matrix3::createRotationMatrix(Scalar angle, Scalar x, Scalar y, Scalar z)
{
#define MAGIC3D_A(row,col)  data[col*3+row]

        Scalar mag, s, c;
        Scalar xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

        s = sin(angle);
        c = cos(angle);

        mag = sqrt( x*x + y*y + z*z );

        // no rotation, return identity matrix
        if (mag == 0.0f) 
        {
            memcpy(this->data, Matrix3::identity, sizeof(Scalar)*3*3);
            return;
        }

        // Rotation matrix is normalized
        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * s;
        ys = y * s;
        zs = z * s;
        one_c = Scalar(1.0f) - c;

        MAGIC3D_A(0,0) = (one_c * xx) + c;
        MAGIC3D_A(0,1) = (one_c * xy) - zs;
        MAGIC3D_A(0,2) = (one_c * zx) + ys;

        MAGIC3D_A(1,0) = (one_c * xy) + zs;
        MAGIC3D_A(1,1) = (one_c * yy) + c;
        MAGIC3D_A(1,2) = (one_c * yz) - xs;

        MAGIC3D_A(2,0) = (one_c * zx) - ys;
        MAGIC3D_A(2,1) = (one_c * yz) + xs;
        MAGIC3D_A(2,2) = (one_c * zz) + c;
        
#undef MAGIC3D_A
}



































