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
/** Implementation file for Matrix4 Generic Implementation
 *
 * @file Matrix4.cc
 * @author Andrew Keating
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
#define MAGIC3D_A(row,col)  data[(col*4)+row]
#define MAGIC3D_B(row,col)  m.data[(col*4)+row]
#define MAGIC3D_P(row,col)  tmp[(col*4)+row]

        Scalar tmp[4*4];
        
        // The math is strong with this one
        // grab a math book if you want to understand this crap
        for (int i = 0; i < 4; i++) 
        {
            Scalar ai0=MAGIC3D_A(i,0),  ai1=MAGIC3D_A(i,1),  ai2=MAGIC3D_A(i,2),  ai3=MAGIC3D_A(i,3);
            MAGIC3D_P(i,0) = ai0 * MAGIC3D_B(0,0) + ai1 * MAGIC3D_B(1,0) + ai2 * MAGIC3D_B(2,0) + ai3 * MAGIC3D_B(3,0);
            MAGIC3D_P(i,1) = ai0 * MAGIC3D_B(0,1) + ai1 * MAGIC3D_B(1,1) + ai2 * MAGIC3D_B(2,1) + ai3 * MAGIC3D_B(3,1);
            MAGIC3D_P(i,2) = ai0 * MAGIC3D_B(0,2) + ai1 * MAGIC3D_B(1,2) + ai2 * MAGIC3D_B(2,2) + ai3 * MAGIC3D_B(3,2);
            MAGIC3D_P(i,3) = ai0 * MAGIC3D_B(0,3) + ai1 * MAGIC3D_B(1,3) + ai2 * MAGIC3D_B(2,3) + ai3 * MAGIC3D_B(3,3);
        }
        
        // copy data back in to make *= operation
        memcpy(data, tmp, sizeof(Scalar)*4*4);
        
#undef MAGIC3D_A
#undef MAGIC3D_B
#undef MAGIC3D_P
}

/// multiply two other matrixes and store the result in this matrix
void Matrix4::multiply(const Matrix4 &m1, const Matrix4 &m2)
{
#define MAGIC3D_A(row,col)  m1.data[(col*4)+row]
#define MAGIC3D_B(row,col)  m2.data[(col*4)+row]
#define MAGIC3D_P(row,col)  data[(col*4)+row]
        
        // The math is strong with this one
        // grab a math book if you want to understand this crap
        for (int i = 0; i < 4; i++) 
        {
            Scalar ai0=MAGIC3D_A(i,0),  ai1=MAGIC3D_A(i,1),  ai2=MAGIC3D_A(i,2),  ai3=MAGIC3D_A(i,3);
            MAGIC3D_P(i,0) = ai0 * MAGIC3D_B(0,0) + ai1 * MAGIC3D_B(1,0) + ai2 * MAGIC3D_B(2,0) + ai3 * MAGIC3D_B(3,0);
            MAGIC3D_P(i,1) = ai0 * MAGIC3D_B(0,1) + ai1 * MAGIC3D_B(1,1) + ai2 * MAGIC3D_B(2,1) + ai3 * MAGIC3D_B(3,1);
            MAGIC3D_P(i,2) = ai0 * MAGIC3D_B(0,2) + ai1 * MAGIC3D_B(1,2) + ai2 * MAGIC3D_B(2,2) + ai3 * MAGIC3D_B(3,2);
            MAGIC3D_P(i,3) = ai0 * MAGIC3D_B(0,3) + ai1 * MAGIC3D_B(1,3) + ai2 * MAGIC3D_B(2,3) + ai3 * MAGIC3D_B(3,3);
        }
        
#undef MAGIC3D_A
#undef MAGIC3D_B
#undef MAGIC3D_P
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


Matrix4 Matrix4::inverse() const
{
	Matrix4 inverse;

	Matrix4 adjoint = this->cofactor().transpose();
	Scalar determinat = this->determinant();

	int r = 4;
	for (int i=0; i < r; i++)
	{
		for (int j=0; j < r; j++)
		{
			inverse.set(i, j, adjoint.get(i, j) / determinat);
		}
	}

	return inverse;
}

Scalar determinant_r(const Scalar a[4][4], Scalar size)
{
	Scalar s=1, det=0, minor[4][4];

	if (size==1)
    {
     return (a[0][0]);
    }
	else
	{
		for (int c=0; c < size; c++)
		{
			int m=0;
			int n=0;
			for (int i=0; i < size; i++)
			{
				for (int j=0; j < size; j++)
				{
					minor[i][j]=0;
					if (i != 0 && j != c)
					{
						minor[m][n] = a[i][j];
						if (n < (size-2))
							n++;
						else
						{
							n=0;
							m++;
						}
					}
				}
			}
			det= det + s * (a[0][c] * determinant_r(minor, size-1));
			s=-1 * s;
		}
	}
 
	return det;
}

Scalar Matrix4::determinant() const
{
	return determinant_r((const Scalar(*)[4])this->data, 4);
}
 
Matrix4 Matrix4::cofactor() const
{
	int f = 4;

	Matrix4 b;
	Matrix4 fac;

	for (int q=0; q<f; q++)
	{
		for (int p=0; p<f; p++)
		{
			int m=0;
			int n=0;
			for (int i=0; i<f; i++)
			{
				for (int j=0; j<f; j++)
				{
					if (i != q && j != p)
					{
						b.set(m, n, this->get(i, j));
						if (n < (f-2))
							n++;
						else
						{
							n=0;
							m++;
						}
					}
				}
			}
			fac.set(q, p, pow((Scalar)-1, (Scalar)q + p) * b.determinant());
		}
	}

	return fac;
}


Matrix4 Matrix4::transpose() const
{
	int r = 4;
	Matrix4 transpose;
 
	for (unsigned int i=0; i < r; i++)
	{
		for (unsigned int j=0; j < r; j++)
		{
			transpose.set(i, j, this->get(j, i));
		}
	}

	return transpose;
}



















