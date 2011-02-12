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
/** Header file for Matrix class
 * 
 * @file Matrix.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MATRIX_H
#define MAGIC3D_MATRIX_H
// 
#include "../Math/Vector.h"
#include "../Exceptions/MagicException.h"
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>


namespace Magic3D
{
	

/** Represents a single Matrix with any
 * number of components. The most popular
 * formats are typedef'ed. N columns, M rows
 */
template<class T, int N, int M>
class Matrix
{
private:
	/// internal data storage, Matrix column major (openGL style)
	T data[N*M];
	
public:
	
	/// default constructor, Identity matrix by default
	inline Matrix()
	{
		for(int i=0, j=0; i < N*M; i++)
		{
			data[i] = (i%M == j) ? (T)1.0f : (T)0.0f;
			if (i%M == 0 && i != 0)
				j++;
		}
	}
	
	/// copy constructor
	inline Matrix(const Matrix<T,N,M>& copy)
	{
		memcpy(this->data, &copy.data, sizeof(T)*N*M);
	}
	
	/** Get a specific column in this matrix
	 * @param v the vector store the selected column in
	 * @param column the column to get
	 */
	inline void getColumn(Vector<T, M>* v, int column)
	{
		v.setInternal(&data[N*column]);
	}
	
};

class Position;


template<class T> class Matrix<T, 4, 4>;

/** Partial specilization for 3x3 matrices
 */
template<class T>
class Matrix<T, 3, 3>
{
private:
	/// internal data storage, Matrix column major (openGL style)
	T data[3*3];
	
	
	/// identity matrix
	static const T identity[];
	
public:
	
	friend class Position;
	friend class Matrix<T,4,4>;
	
	/// default constructor, Identity matrix by default
	inline Matrix()
	{
		memcpy(this->data, Matrix<T,3,3>::identity, sizeof(T)*3*3);
	}
	
	/// copy constructor
	inline Matrix(const Matrix<T,3,3>& copy)
	{
		this->set(copy);
	}
	
	/// array constructor
	inline Matrix(const T* array)
	{
		memcpy(this->data, array, sizeof(T)*3*3);
	}
	
	/** Set this matrix to another matrix
	 * @param copy the matrix to copy
	 */
	inline void set(const Matrix<T,3,3>& copy)
	{
		memcpy(this->data, &copy.data, sizeof(T)*3*3);
	}
	
	/** Get internal data for this matrix
	 * @return the internal data for this matrix in opengl format
	 */
	inline const T* getInternal()
	{
		return data;
	}
	
	/** Get a specific column in this matrix
	 * @param v the vector store the selected column in
	 * @param column the column to get
	 */
	inline void getColumn(Vector<T, 3>* v, int column)
	{
		v.setInternal(&data[3*column]);
	}
	
	/** Multiply a matrix by this matrix
	 * @param m the matrix to multiply to this one
	 */
	inline void multiply(const Matrix<T,3,3>& m)
	{
#define MAGIC3D_A(row,col)  data[(col*3)+row]
#define MAGIC3D_B(row,col)  m.data[(col*3)+row]
#define MAGIC3D_P(row,col)  tmp[(col*3)+row]

		T tmp[3*3];
		
		// The math is strong with this one
		// grab a math book if you want to understand this crap
		for (int i = 0; i < 3; i++) 
		{
			T ai0=MAGIC3D_A(i,0),  ai1=MAGIC3D_A(i,1),  ai2=MAGIC3D_A(i,2);
			MAGIC3D_P(i,0) = ai0 * MAGIC3D_B(0,0) + ai1 * MAGIC3D_B(1,0) + ai2 * MAGIC3D_B(2,0);
			MAGIC3D_P(i,1) = ai0 * MAGIC3D_B(0,1) + ai1 * MAGIC3D_B(1,1) + ai2 * MAGIC3D_B(2,1);
			MAGIC3D_P(i,2) = ai0 * MAGIC3D_B(0,2) + ai1 * MAGIC3D_B(1,2) + ai2 * MAGIC3D_B(2,2);
		}
		
		// copy data back in to make *= operation
		memcpy(data, tmp, sizeof(T)*3*3);
		
#undef MAGIC3D_A
#undef MAGIC3D_B
#undef MAGIC3D_P
	}
	
	/** Multiply two matrices and store the result in this matrix
	 * @param m1 the first matrix to multiply
	 * @param m2 the second matrix to multiply
	 */
	inline void multiply(const Matrix<T,3,3>& m1, const Matrix<T,3,3>& m2)
	{
#define MAGIC3D_A(row,col)  m1.data[(col*3)+row]
#define MAGIC3D_B(row,col)  m2.data[(col*3)+row]
#define MAGIC3D_P(row,col)  data[(col*3)+row]
		
		// The math is strong with this one
		// grab a math book if you want to understand this crap
		for (int i = 0; i < 3; i++) 
		{
			T ai0=MAGIC3D_A(i,0),  ai1=MAGIC3D_A(i,1),  ai2=MAGIC3D_A(i,2);
			MAGIC3D_P(i,0) = ai0 * MAGIC3D_B(0,0) + ai1 * MAGIC3D_B(1,0) + ai2 * MAGIC3D_B(2,0);
			MAGIC3D_P(i,1) = ai0 * MAGIC3D_B(0,1) + ai1 * MAGIC3D_B(1,1) + ai2 * MAGIC3D_B(2,1);
			MAGIC3D_P(i,2) = ai0 * MAGIC3D_B(0,2) + ai1 * MAGIC3D_B(1,2) + ai2 * MAGIC3D_B(2,2);
		}
		
#undef MAGIC3D_A
#undef MAGIC3D_B
#undef MAGIC3D_P
	}
	
	/** Turn this matrix into a scaling matrix
	 * @param xScale the x factor (hehe...)
	 * @param yScale the y factor
	 * @param zScale the z factor
	 */
	inline void createScaleMatrix(T xScale, T yScale, T zScale)
	{
		// load identity matrix
		memcpy(this->data, Matrix<T,3,3>::identity, sizeof(T)*3*3);
		
		// apply scaling
		data[0] = xScale; 
		data[4] = yScale; 
		data[8] = zScale;
	}
	
	/** Turn this matrix into a rotation matrix
	 * @param angle the angle to rotate
	 * @param x the x part of the unit vector to rotate around
	 * @param y the y part of the unit vector to rotate around
	 * @param z the z part of the unit vector to rotate around
	 */
	inline void createRotationMatrix(T angle, T x, T y, T z)
	{
#define MAGIC3D_A(row,col)  data[col*3+row]

		T mag, s, c;
		T xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

		s = T(sin(angle));
		c = T(cos(angle));

		mag = T(sqrt( x*x + y*y + z*z ));

		// no rotation, return identity matrix
		if (mag == 0.0f) 
		{
			memcpy(this->data, Matrix<T,3,3>::identity, sizeof(T)*3*3);
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
		one_c = T(1.0f) - c;

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
	
	
};

template<class T>
const T Matrix<T,3,3>::identity[] = {1.0f, 0.0f, 0.0f, 
						             0.0f, 1.0f, 0.0f,
						             0.0f, 0.0f, 1.0f};

// point forward declaration
template<class T, int N> class Point;
//template<class T, int N> class Vector;

// position forward declaration
class Position;

									 
/** Partial specilization for 4x4 matrices
 */
template<class T>
class Matrix<T, 4, 4>
{
private:
	/// internal data storage, Matrix column major (openGL style)
	T data[4*4];
	
	/// identity matrix
	static const T identity[];
	
public:
	template<class E, int N>
	friend class Point;
	
	template<class E, int N>
	friend class Vector;
	
	friend class Position;
	
	/// default constructor, Identity matrix by default
	inline Matrix()
	{
		memcpy(this->data, Matrix<T,4,4>::identity, sizeof(T)*4*4);
	}
	
	/// array constructor
	inline Matrix(const T* array)
	{
		memcpy(this->data, array, sizeof(T)*4*4);
	}
	
	/// copy constructor
	inline Matrix(const Matrix<T,4,4>& copy)
	{
		this->set(copy);
	}
	
	/** set this matrix to another
	 * @param copy the matrix to copy
	 */
	inline void set(const Matrix<T,4,4>& copy)
	{
		memcpy(this->data, &copy.data, sizeof(T)*4*4);
	}
	
	/** Get a read-only reference to the internal data
	 * @return internal data array
	 */
	inline const T* getInternal()
	{
		return data;
	}
	
	/** Get a specific column in this matrix
	 * @param v the vector store the selected column in
	 * @param column the column to get
	 */
	inline void getColumn(Vector<T, 4>* v, int column)
	{
		v.setInternal(&data[4*column]);
	}
	
	/** Multiply a matrix by this matrix
	 * @param m the matrix to multiply to this one
	 */
	inline void multiply(const Matrix<T,4,4>& m)
	{
#define MAGIC3D_A(row,col)  data[(col<<2)+row]
#define MAGIC3D_B(row,col)  m.data[(col<<2)+row]
#define MAGIC3D_P(row,col)  tmp[(col<<2)+row]

		T tmp[4*4];
		
		// The math is strong with this one
		// grab a math book if you want to understand this crap
		for (int i = 0; i < 4; i++) 
		{
			T ai0=MAGIC3D_A(i,0),  ai1=MAGIC3D_A(i,1),  ai2=MAGIC3D_A(i,2),  ai3=MAGIC3D_A(i,3);
			MAGIC3D_P(i,0) = ai0 * MAGIC3D_B(0,0) + ai1 * MAGIC3D_B(1,0) + ai2 * MAGIC3D_B(2,0) + ai3 * MAGIC3D_B(3,0);
			MAGIC3D_P(i,1) = ai0 * MAGIC3D_B(0,1) + ai1 * MAGIC3D_B(1,1) + ai2 * MAGIC3D_B(2,1) + ai3 * MAGIC3D_B(3,1);
			MAGIC3D_P(i,2) = ai0 * MAGIC3D_B(0,2) + ai1 * MAGIC3D_B(1,2) + ai2 * MAGIC3D_B(2,2) + ai3 * MAGIC3D_B(3,2);
			MAGIC3D_P(i,3) = ai0 * MAGIC3D_B(0,3) + ai1 * MAGIC3D_B(1,3) + ai2 * MAGIC3D_B(2,3) + ai3 * MAGIC3D_B(3,3);
		}
		
		// copy data back in to make *= operation
		memcpy(data, tmp, sizeof(T)*4*4);
		
#undef MAGIC3D_A
#undef MAGIC3D_B
#undef MAGIC3D_P
	}
	
	
	/** Multiply two matrices together and store the result in this matrix
	 * @param m1 the first matrix to multiply
	 * @param m2 the second to matrix to multipy
	 */
	inline void multiply(const Matrix<T,4,4>& m1, const Matrix<T,4,4>& m2)
	{
#define MAGIC3D_A(row,col)  m1.data[(col<<2)+row]
#define MAGIC3D_B(row,col)  m2.data[(col<<2)+row]
#define MAGIC3D_P(row,col)  data[(col<<2)+row]
		
		// The math is strong with this one
		// grab a math book if you want to understand this crap
		for (int i = 0; i < 4; i++) 
		{
			T ai0=MAGIC3D_A(i,0),  ai1=MAGIC3D_A(i,1),  ai2=MAGIC3D_A(i,2),  ai3=MAGIC3D_A(i,3);
			MAGIC3D_P(i,0) = ai0 * MAGIC3D_B(0,0) + ai1 * MAGIC3D_B(1,0) + ai2 * MAGIC3D_B(2,0) + ai3 * MAGIC3D_B(3,0);
			MAGIC3D_P(i,1) = ai0 * MAGIC3D_B(0,1) + ai1 * MAGIC3D_B(1,1) + ai2 * MAGIC3D_B(2,1) + ai3 * MAGIC3D_B(3,1);
			MAGIC3D_P(i,2) = ai0 * MAGIC3D_B(0,2) + ai1 * MAGIC3D_B(1,2) + ai2 * MAGIC3D_B(2,2) + ai3 * MAGIC3D_B(3,2);
			MAGIC3D_P(i,3) = ai0 * MAGIC3D_B(0,3) + ai1 * MAGIC3D_B(1,3) + ai2 * MAGIC3D_B(2,3) + ai3 * MAGIC3D_B(3,3);
		}
		
#undef MAGIC3D_A
#undef MAGIC3D_B
#undef MAGIC3D_P
	}
	
	/** Turn this matrix into a scaling matrix
	 * @param xScale the x factor (hehe...)
	 * @param yScale the y factor
	 * @param zScale the z factor
	 */
	inline void createScaleMatrix(T xScale, T yScale, T zScale)
	{
		// load identity matrix
		memcpy(this->data, Matrix<T,4,4>::identity, sizeof(T)*4*4);
		
		// apply scaling
		data[0] = xScale; 
		data[5] = yScale; 
		data[10] = zScale;
	}
	
	/** Turn this matrix into a perspective transform matrix
	 * @param fov the allowable field of view
	 * @param aspect the aspect ratio to use
	 * @param zMin the minimum z depth
	 * @param zMax the maximum z depth
	 */
	inline void createPerspectiveMatrix(T fov, T aspect, T zMin, T zMax)
	{
		// load identity matrix
		memcpy(this->data, Matrix<T,4,4>::identity, sizeof(T)*4*4);

		T yMax = zMin * tan(double(fov * M_PI/360));
		T yMin = -yMax;
		T xMin = yMin * aspect;
		T xMax = -xMin; 
		
		data[0] = (2.0f * zMin) / (xMax - xMin);
		data[5] = (2.0f * zMin) / (yMax - yMin);
		data[8] = (xMax + xMin) / (xMax - xMin);
		data[9] = (yMax + yMin) / (yMax - yMin);
		data[10] = -((zMax + zMin) / (zMax - zMin));
		data[11] = -1.0f;
		data[14] = -((2.0f * (zMax*zMin))/(zMax - zMin));
		data[15] = 0.0f;
	}
	
	/** Turn this matrix into a orthographic transform matrix
	* @param xMin the min x bound
	* @param xMax the max x bound
	* @param yMin the min y bound
	* @param yMax the max y bound
	* @param zMin the min z bound
	* @param zMax the max z bound
	*/
	
	inline void createOrthographicMatrix(T xMin, T xMax, T yMin, T yMax, T zMin, T zMax)
	{
		// load identity matrix
		memcpy(this->data, Matrix<T,4,4>::identity, sizeof(T)*4*4);
	
		data[0] = 2.0f / (xMax - xMin);
		data[5] = 2.0f / (yMax - yMin);
		data[10] = -2.0f / (zMax - zMin);
		data[12] = -((xMax + xMin)/(xMax - xMin));
		data[13] = -((yMax + yMin)/(yMax - yMin));
		data[14] = -((zMax + zMin)/(zMax - zMin));
		data[15] = 1.0f;
	}
	
	/** Turn this matrix into a rotation matrix
	 * @param angle the angle to rotate
	 * @param x the x part of the unit vector to rotate around
	 * @param y the y part of the unit vector to rotate around
	 * @param z the z part of the unit vector to rotate around
	 */
	inline void createRotationMatrix(T angle, T x, T y, T z)
	{
#define MAGIC3D_A(row,col)  data[col*4+row]
		T mag, s, c;
		T xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

		s = T(sin(angle));
		c = T(cos(angle));

		mag = T(sqrt( x*x + y*y + z*z ));

		// no rotation, return identity matrix
		if (mag == 0.0f) 
		{
			memcpy(this->data, Matrix<T,4,4>::identity, sizeof(T)*4*4);
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
		one_c = T(1.0f) - c;

		MAGIC3D_A(0,0) = (one_c * xx) + c;
		MAGIC3D_A(0,1) = (one_c * xy) - zs;
		MAGIC3D_A(0,2) = (one_c * zx) + ys;
		MAGIC3D_A(0,3) = T(0.0f);

		MAGIC3D_A(1,0) = (one_c * xy) + zs;
		MAGIC3D_A(1,1) = (one_c * yy) + c;
		MAGIC3D_A(1,2) = (one_c * yz) - xs;
		MAGIC3D_A(1,3) = T(0.0f);

		MAGIC3D_A(2,0) = (one_c * zx) - ys;
		MAGIC3D_A(2,1) = (one_c * yz) + xs;
		MAGIC3D_A(2,2) = (one_c * zz) + c;
		MAGIC3D_A(2,3) = T(0.0f);

		MAGIC3D_A(3,0) = T(0.0f);
		MAGIC3D_A(3,1) = T(0.0f);
		MAGIC3D_A(3,2) = T(0.0f);
		MAGIC3D_A(3,3) = T(1.0f);
		
#undef MAGIC3D_A
	}
	
	/** Turn this matrix into a translation transform matrix
	 * @param x the x ammount to move
	 * @param y the x ammount to move
	 * @param z the x ammount to move
	 */
	inline void createTranslationMatrix(T x, T y, T z)
	{
		//load identity
		memcpy(this->data, Matrix<T,4,4>::identity, sizeof(T)*4*4);
		
		data[12] = x; 
		data[13] = y; 
		data[14] = z;
	}
	
	/** Extract the normalized rotational component out of this matrix
	 * @param matrix the matrix to store the rotational component into
	 */
	inline void extractRotation(Matrix<T,3,3>& matrix)
	{
		// copy the upper left 3x3 matrix, removing translation
		memcpy(matrix.data,     data,     sizeof(T) * 3);
		memcpy(matrix.data + 3, data + 4, sizeof(T) * 3);
		memcpy(matrix.data + 6, data + 8, sizeof(T) * 3);
	}
	
	
};
	
template<class T>
const T Matrix<T,4,4>::identity[] = {1.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, 1.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 1.0f};
	








// popular formats
typedef Matrix<float, 3, 3> Matrix33f;
typedef Matrix<double, 3, 3> Matrix33d;
typedef Matrix<float, 4, 4> Matrix44f;
typedef Matrix<double, 4, 4> Matrix44d;






};






#endif





















