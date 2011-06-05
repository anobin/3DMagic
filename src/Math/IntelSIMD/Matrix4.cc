/* 
Copyright (c) 2011 Matthew Del Buono
Based on the generic implementation by Andrew Keating

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
/** Implementation file for Matrix4 Intel SIMD Implementation
 *
 * @file Matrix4.cc
 * @author Matthew Del Buono 
 */

#include <Math/Generic/Matrix4.h>


/// the identity matrix
const Scalar Matrix4::identity[] = {1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f};

/// create a scale matrix
void Matrix4::createScaleMatrix(Scalar x, Scalar y, Scalar z)
{
    // load identity matrix
    memcpy(this->data, Matrix4::identity, sizeof(Scalar)*4*4);
    
    // apply scaling
    data[0] = x; 
    data[5] = y; 
    data[10] = z;
}

/// multiply this matrix and another matrix
void Matrix4::multiply(const Matrix4 &m)
{
    return multiply(*this, m);
}

/// multiply two other matrixes and store the result in this matrix
__declspec(naked)
void Matrix4::multiply(const Matrix4 &m1, const Matrix4 &m2)
{
    // Transpose matrix 1 to assist with SSE
    __asm 
    {
        // Recommend caching of m2 while we deal with m1
        MOV         eax,        m2
        // MOV      ecx,        this        Guaranteed by function call
        PREFETCHT0  [eax]

        // Resolve reference
        MOV         eax,        m1

        
        MOVAPS      xmm0,       [eax]m1.data                        // A1 A2 A3 A4
        MOVAPS      xmm1,       [eax]m1.data + (4  * TYPE Scalar)   // B1 B2 B3 B4
        MOVAPS      xmm2,       [eax]m1.data + (8  * TYPE Scalar)   // C1 C2 C3 C4
        MOVAPS      xmm3,       [eax]m1.data + (12 * TYPE Scalar)   // D1 D2 D3 D4

        // Transpose for ease of SSE
        /* Shift to the following:
                A1  C1  A2  C2              A1  B1  C1  D1
                A3  C3  A4  C4              A2  B2  C2  D2
                B1  D1  B2  D2      Then    A3  B3  C3  D3
                B3  D3  B4  D4              A4  B4  C4  D4
        */

        MOVAPS      xmm5,       xmm0    // A1 A2 A3 A4
        MOVAPS      xmm6,       xmm1    // B1 B2 B3 B4
        UNPCKHPS    xmm0,       xmm2    // A1 C1 A2 C2
        UNPCKLPS    xmm5,       xmm2    // A3 C3 A4 C4
        UNPCKHPS    xmm1,       xmm3    // B1 D1 B2 D2
        UNPCKLPS    xmm6,       xmm3    // B3 D3 B4 D4

        // Step one complete:
        // xmm0, xmm5, xmm1, xmm6
        MOVAPS      xmm2,       xmm0    // A1 C1 A2 C2
        MOVAPS      xmm3,       xmm6    // B3 D3 B4 D4
        UNPCKHPS    xmm0,       xmm1    // A1 B1 C1 D1
        UNPCKLPS    xmm2,       xmm1    // A2 B2 C2 D2
        UNPCKHPS    xmm5,       xmm6    // A3 B3 C3 D3
        UNPCKLPS    xmm3,       xmm6    // A4 B4 C4 D4        

        // Transpose complete (xmm0 xmm2 xmm5 xmm3). Do multiply
        // Grab first row of first matrix, multiply with first column of second matrix
        MOV         eax,        m2
        MOVAPS      xmm1,       [eax]m2.data
        MOVAPS      xmm4,       xmm0    // xmm4 = m2 first col
        MULPS       xmm0,       xmm1    // xmm0 = m1 first row * m2 first col
        MOVAPS      xmm6,       xmm2
        MULPS       xmm6,       xmm1    // xmm6 = m1 first row * m2 second col
        HADDPS      xmm0,       xmm6    // xmm0 = a bunch of crap

        MOVAPS      xmm6,       xmm1
        MULPS       xmm6,       xmm5    // xmm6 = m1 first row * m2 third col
        MOVAPS      xmm7,       xmm1
        MULPS       xmm7,       xmm3    // xmm7 = m1 first row * m2 fourth col
        HADDPS      xmm6,       xmm7    // xmm6 = another bunch of crap

        HADDPS      xmm0,       xmm6    // bunch of crap + bunch of crap = first result row complete
        MOVAPS      [ecx]m2.data, xmm0  // store back in m1

        // Do other rows
#define COMPUTE_ROW(n) \
        __asm MOVAPS        xmm1,       [eax]m2.data + (n * 4 * TYPE Scalar)    \
        __asm MOVAPS        xmm0,       xmm4                                    \
        __asm MULPS         xmm0,       xmm1                                    \
        __asm MOVAPS        xmm6,       xmm2                                    \
        __asm MULPS         xmm6,       xmm1                                    \
        __asm HADDPS        xmm0,       xmm6                                    \
                                                                                \
        __asm MOVAPS        xmm6,       xmm5                                    \
        __asm MULPS         xmm6,       xmm1                                    \
        __asm MOVAPS        xmm7,       xmm3                                    \
        __asm MULPS         xmm7,       xmm1                                    \
        __asm HADDPS        xmm6,       xmm7                                    \
                                                                                \
        __asm HADDPS        xmm0,       xmm6                                    \
        __asm MOVAPS        [ecx]m2.data + (n * 4 * TYPE Scalar), xmm0          \


        // Do the other rows
        COMPUTE_ROW(1);
        COMPUTE_ROW(2);
        COMPUTE_ROW(3);

#undef COMPUTE_ROW
    }
}

/// create a perepective matrix
void Matrix4::createPerspectiveMatrix(Scalar fov, Scalar aspect, Scalar zMin, Scalar zMax)
{
    // load identity matrix
    memcpy(this->data, Matrix4::identity, sizeof(Scalar)*4*4);

    Scalar yMax = Scalar(zMin * tan(fov * M_PI/360));
    Scalar yMin = -yMax;
    Scalar xMin = yMin * aspect;
    Scalar xMax = -xMin; 
    
    data[0] = (2.0f * zMin) / (xMax - xMin);
    data[5] = (2.0f * zMin) / (yMax - yMin);
    data[8] = (xMax + xMin) / (xMax - xMin);
    data[9] = (yMax + yMin) / (yMax - yMin);
    data[10] = -((zMax + zMin) / (zMax - zMin));
    data[11] = -1.0f;
    data[14] = -((2.0f * (zMax*zMin))/(zMax - zMin));
    data[15] = 0.0f;
}

/// create a orthographic matrix
void Matrix4::createOrthographicMatrix(Scalar xMin, Scalar xMax, Scalar yMin, Scalar yMax, Scalar zMin, Scalar zMax)
{
    // load identity matrix
    memcpy(this->data, Matrix4::identity, sizeof(Scalar)*4*4);

    data[0] = 2.0f / (xMax - xMin);
    data[5] = 2.0f / (yMax - yMin);
    data[10] = -2.0f / (zMax - zMin);
    data[12] = -((xMax + xMin)/(xMax - xMin));
    data[13] = -((yMax + yMin)/(yMax - yMin));
    data[14] = -((zMax + zMin)/(zMax - zMin));
    data[15] = 1.0f;
}

/// create rotation matrix
void Matrix4::createRotationMatrix(Scalar angle, Scalar x, Scalar y, Scalar z)
{
#define MAGIC3D_A(row,col)  data[(col*4)+row]
        Scalar mag, s, c;
        Scalar xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

        s = sin(angle);
        c = cos(angle);

        mag = sqrt( x*x + y*y + z*z );

        // no rotation, return identity matrix
        if (mag == 0.0f) 
        {
            memcpy(this->data, Matrix4::identity, sizeof(Scalar)*4*4);
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
        MAGIC3D_A(0,3) = 0.0f;

        MAGIC3D_A(1,0) = (one_c * xy) + zs;
        MAGIC3D_A(1,1) = (one_c * yy) + c;
        MAGIC3D_A(1,2) = (one_c * yz) - xs;
        MAGIC3D_A(1,3) = 0.0f;

        MAGIC3D_A(2,0) = (one_c * zx) - ys;
        MAGIC3D_A(2,1) = (one_c * yz) + xs;
        MAGIC3D_A(2,2) = (one_c * zz) + c;
        MAGIC3D_A(2,3) = 0.0f;

        MAGIC3D_A(3,0) = 0.0f;
        MAGIC3D_A(3,1) = 0.0f;
        MAGIC3D_A(3,2) = 0.0f;
        MAGIC3D_A(3,3) = 1.0f;
        
#undef MAGIC3D_A
}

/// create a translation matrix
void Matrix4::createTranslationMatrix(Scalar x, Scalar y, Scalar z)
{
    //load identity
    memcpy(this->data, Matrix4::identity, sizeof(Scalar)*4*4);
    
    data[12] = x; 
    data[13] = y; 
    data[14] = z;
}

/// extract the rotational component out of this matrix
void Matrix4::extractRotation(Matrix3& out)
{
    // copy the upper left 3x3 matrix, removing translation
    memcpy(out.data,     data,     sizeof(Scalar) * 3);
    memcpy(out.data + 3, data + 4, sizeof(Scalar) * 3);
    memcpy(out.data + 6, data + 8, sizeof(Scalar) * 3);
}























