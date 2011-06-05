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

#include <Math/Intel/Matrix4.h>


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
void Matrix4::multiply(const Matrix4 &m1, const Matrix4 &m2)
{
	
#ifdef _MSC_VER
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

#elif defined(__GNUC__) 
	
#define COMPUTE_ROW(n) \
	__asm__ ( \
	".intel_syntax noprefix\n\t" \
	"MOVAPS        xmm1,       [eax]m2.data + (n * 4 * TYPE Scalar)\n\t"    \
	"MOVAPS        xmm0,       xmm4\n\t"                                    \
	"MULPS         xmm0,       xmm1\n\t"                                    \
	"MOVAPS        xmm6,       xmm2\n\t"                                    \
	"MULPS         xmm6,       xmm1\n\t"                                    \
	"HADDPS        xmm0,       xmm6\n\t"                                    \
																			\
	"MOVAPS        xmm6,       xmm5\n\t"                                    \
	"MULPS         xmm6,       xmm1\n\t"                                    \
	"MOVAPS        xmm7,       xmm3\n\t"                               \
	"MULPS         xmm7,       xmm1\n\t"                               \
	"HADDPS        xmm6,       xmm7\n\t"                               \
																	        \
	"HADDPS        xmm0,       xmm6\n\t"                               \
	"MOVAPS        [ecx]m2.data + (n * 4 * TYPE Scalar), xmm0\n\t");          
	
	asm
    (
        ".intel_syntax noprefix\n\t"
		// Recommend caching of m2 while we deal with m1
        "MOV         eax,        m2\n\t"
		
        "PREFETCHT0  [eax] \n\t"

		//Resolve reference
        "MOV         eax,        m1\n\t"

        
        "MOVAPS      xmm0,       [eax + loc]\n\t"                  // A1 A2 A3 A4
        "MOVAPS      xmm1,       [eax + loc + (4  * 4)]\n\t"   // B1 B2 B3 B4
        "MOVAPS      xmm2,       [eax + loc + (8  * 4)]\n\t"   // C1 C2 C3 C4
        "MOVAPS      xmm3,       [eax + loc + (12 * 4)]\n\t"   // D1 D2 D3 D4

        // Transpose for ease of SSE
        /* Shift to the following:
                A1  C1  A2  C2              A1  B1  C1  D1
                A3  C3  A4  C4              A2  B2  C2  D2
                B1  D1  B2  D2      Then    A3  B3  C3  D3
                B3  D3  B4  D4              A4  B4  C4  D4
        */

        "MOVAPS      xmm5,       xmm0\n\t"    // A1 A2 A3 A4
        "MOVAPS      xmm6,       xmm1\n\t"    // B1 B2 B3 B4
        "UNPCKHPS    xmm0,       xmm2\n\t"    // A1 C1 A2 C2
        "UNPCKLPS    xmm5,       xmm2\n\t"    // A3 C3 A4 C4
        "UNPCKHPS    xmm1,       xmm3\n\t"    // B1 D1 B2 D2
        "UNPCKLPS    xmm6,       xmm3\n\t"    // B3 D3 B4 D4

        // Step one complete:
        // xmm0, xmm5, xmm1, xmm6
        "MOVAPS      xmm2,       xmm0\n\t"    // A1 C1 A2 C2
        "MOVAPS      xmm3,       xmm6\n\t"    // B3 D3 B4 D4
        "UNPCKHPS    xmm0,       xmm1\n\t"    // A1 B1 C1 D1
        "UNPCKLPS    xmm2,       xmm1\n\t"    // A2 B2 C2 D2
        "UNPCKHPS    xmm5,       xmm6\n\t"    // A3 B3 C3 D3
        "UNPCKLPS    xmm3,       xmm6\n\t"    // A4 B4 C4 D4        

        // Transpose complete (xmm0 xmm2 xmm5 xmm3). Do multiply
        // Grab first row of first matrix, multiply with first column of second matrix
        "MOV         eax,        m2\n\t"
        "MOVAPS      xmm1,       [eax + loc]\n\t"
        "MOVAPS      xmm4,       xmm0\n\t"    // xmm4 = m2 first col
        "MULPS       xmm0,       xmm1\n\t"    // xmm0 = m1 first row * m2 first col
        "MOVAPS      xmm6,       xmm2\n\t"
        "MULPS       xmm6,       xmm1\n\t"    // xmm6 = m1 first row * m2 second col
        "HADDPS      xmm0,       xmm6\n\t"    // xmm0 = a bunch of crap

        "MOVAPS      xmm6,       xmm1\n\t"
        "MULPS       xmm6,       xmm5\n\t"    // xmm6 = m1 first row * m2 third col
        "MOVAPS      xmm7,       xmm1\n\t"
        "MULPS       xmm7,       xmm3\n\t"    // xmm7 = m1 first row * m2 fourth col
        "HADDPS      xmm6,       xmm7\n\t"    // xmm6 = another bunch of crap

        "HADDPS      xmm0,       xmm6\n\t"    // bunch of crap + bunch of crap = first result row complete
        "MOVAPS      [ecx + loc], xmm0\n\t"  // store back in m1
		: /* no outputs*/
		: [loc] "g" (&m1.data)
	);
        // Do the other rows
        //COMPUTE_ROW(1);
        //COMPUTE_ROW(2);
        //COMPUTE_ROW(3);
#undef COMPUTE_ROW
		
#else
#error Unrecognized compiler - cannot determine how to inline assembly
#endif
}

/// create a perspective matrix
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
    ALIGN(16, static Scalar ones[4]) = {1.0f, 1.0f, 1.0f, 1.0f};
    ALIGN(16, static Scalar finalColumn[4]) = {0.0f, 0.0f, 0.0f, 1.0f};

    struct                              // makes sure compiler doesn't do strange tricks like
    {                                   // buffer overflow bytes
        ALIGN(16, Scalar buffer[4]);
        Scalar angleAligned;            // guaranteed to be one byte after buffer
    } segm;

    ALIGN(16, Scalar sineAngle);
    ALIGN(16, Scalar cosineAngle);

    // Will be backwards, because stack grows up
    // w | z | y | x
    segm.buffer[0] = 0; // setting this to 0 first for easier computation of magnitude. w should be 1
    memcpy(segm.buffer + 1, &z, sizeof(Scalar) * 4); // goes straight through to angleAligned
                                                // TODO: Make sure we need an aligned angle
    __asm 
    {
        PREFETCHT0  segm.buffer
        FLD         segm.angleAligned
        FSINCOS                                 // sin @ ST(1), cos @ ST(0)
         
        FSTP        DWORD PTR cosineAngle
        FSTP        DWORD PTR sineAngle

        PREFETCHT0  ones
        // Compute magnitude
        MOVAPS      xmm0,       segm.buffer
        MOVAPS      xmm4,       xmm0                    // xmm4 = 0 z y x
        MULPS       xmm0,       xmm0                    // square
        PSHUFD      xmm1,       xmm0, 10010011b
        PSHUFD      xmm2,       xmm0, 01001110b
        PSHUFD      xmm3,       xmm1, 01001110b         // (same third op is correct, see diagram)
        ADDPS       xmm0,       xmm1 
        ADDPS       xmm2,       xmm3
        ADDPS       xmm0,       xmm2
        SQRTPS      xmm0,       xmm0                    // xmm0 = magnitude, 4 times

        // Scale
        DIVPS       xmm0,       xmm4                    // xmm0 = normalized (0, z, y, x)
        PSHUFD      xmm1,       xmm0, 11010010b         // Shuffled copy     (0, y, x, z)

        // xx, xy, zx, xy, yy, yz, zx, yz, zz
        MOVAPS      xmm2,       xmm0                    // xmm2 = 0, z, y, x
        MULPS       xmm1,       xmm0                    // xmm1 = 0, zy, xy, xz
        MOVAPS      xmm3,       xmm2                    
        MULPS       xmm0,       xmm0                    // xmm0 = 0, zz, yy, xx

        // xmm4 = 1 - cos(angle), 4 times
        MOVSS       xmm5,       cosineAngle
        MOVAPS      xmm4,       ones
        PSHUFD      xmm5,       xmm5, 00000000h
        SUBPS       xmm4,       xmm5
        MOVAPS      xmm7,       xmm5                    // xmm7 = cos(angle), 4 times

        // xmm5 = sin(angle), 4 times
        MOVSS       xmm5,       sineAngle
        PSHUFD      xmm5,       xmm5, 00000000h

        // Final math parameters
        MOVAPS      xmm3,       xmm4
        MULPS       xmm5,       xmm2                    // xmm5 = 0, zs, ys, xs
        MULPS       xmm4,       xmm1                    // xmm4 = 0, czy, cxy, cxz
        MULPS       xmm3,       xmm0                    // xmm3 = 0, czz, cyy, cxx
        PSHUFD      xmm5,       xmm5, 10010011b         // xmm5 = zs, ys, xs, 0
        MOVSS       xmm5,       xmm7                    // xmm5 = zs, ys, xs, c

        // Start computations. xmm0, xmm6, and xmm2 are available as temporaries
        // xmm1 = 0, zy, xy, xz
        // xmm3 = 0, czz, cyy, cxx
        // xmm4 = 0, czy, cxy, cxz
        // xmm5 = zs, ys, xs, c
        // xmm7 = cosa, cosa, cosa, cosa
        PREFETCHT0  finalColumn
        MOV         edx,        data

        // Column 1: cxx + c, cxy + zs, czx - ys, 0
        MOVAPS      xmm6,       xmm4
        UNPCKLPS    xmm6,       xmm3                    // xmm6 = cyy, cxy, cxx, cxz
        PSHUFD      xmm2,       xmm7, 00101100b         // xmm2 = c, ys, zs, c
        PSHUFD      xmm6,       xmm6, 01001000b         // xmm6 = cxx, czx, cxy, czx
        MOVSS       xmm2,       xmm3                    // xmm2 = c, ys, zs, czx
        ADDSUBPS    xmm6,       xmm2                    // xmm6 = cxx + c, czx - ys, cxy + zs, 0
        PSHUFD      xmm6,       xmm6, 11011000b         // xmm6 = column 1
        MOVAPS      [edx + Matrix4::data],                          xmm6

        // Column 2: cxy - zs, cyy + c, cyz + xs, 0
        PSHUFD      xmm0,       xmm4, 11000110b         // xmm0 = 0, cxz, cxy, cyz
        UNPCKLPS    xmm0,       xmm3                    // xmm0 = cyy, cxy, cxx, cyz
        PSHUFD      xmm2,       xmm5, 00110101b         // xmm2 = c, zs, xs, xs
        PSHUFD      xmm0,       xmm0, 11100001b         // xmm0 = cyy, cxy, cyz, cxx
        MOVSS       xmm0,       xmm2                    // xmm0 = cyy, cxy, cyz, xs
        ADDSUBPS    xmm0,       xmm2                    // xmm0 = cyy + c, cxy - zs, cyz + xs, 0
        PSHUFD      xmm0,       xmm0, 10110100b         // xmm0 = column 2
        MOVAPS      [edx + Matrix4::data + 4 * TYPE Scalar],        xmm0

        // Column 3: czx + ys, cyz - xs, czz + c, 0
        PSHUFD      xmm6,       xmm4, 00100010b         // xmm6 = cxz, cyz, cxz, cyz
        UNPCKHPS    xmm6,       xmm3                    // xmm6 = 0, cxz, czz, cyz
        PSHUFD      xmm2,       xmm5, 10010000b         // xmm2 = ys, xs, c, c
        PSHUFD      xmm6,       xmm6, 10000111b         // xmm6 = czx, cyz, czz, 0 
        MOVSS       xmm2,       xmm6                    // xmm2 = ys, xs, c, 0
        ADDSUBPS    xmm6,       xmm2                    // xmm6 = czx + ys, cyz - xs, czz + c, 0
        MOVAPS      [edx + Matrix4::data + 8 * TYPE Scalar],        xmm6

        MOVAPS      xmm0,       finalColumn
        MOVAPS      [edx + Matrix4::data + 12 * TYPE Scalar],       xmm0
    }
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























