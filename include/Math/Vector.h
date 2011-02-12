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
/** Header file for the Vector class
 *
 * @file Vector.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VECTOR_H
#define MAGIC3D_VECTOR_H


#include "../Exceptions/MagicException.h"
#include <string.h>
#include <math.h>


namespace Magic3D
{

// forward declaration of Matrix
template<class T, int N, int M> class Matrix;
	
	
/** Represents a single vector with any number
 * of components, the most popular formats are 
 * typedef'ed
 */
template <class T, int N>
class Vector
{
private:
	/// internal storage
	T data[N];
	
public:	
	
	/// default constructor, all components 0
	inline Vector()
	{
		for (int i=0; i < N; i++)
			data[i] = (T)0.0f; // most likey a float type
	} 
	
	/// array constructor
	inline Vector(T* array)
	{
		for (int i=0; i < N; i++)
			data[i] = array[i];
	}
	
	/// copy constructor
	inline Vector(const Vector<T,N>& copy)
	{
		memcpy(&data, &copy.data, sizeof(T)*N);
	}
	
	/** Get read-only access to internal data
	 * @return internal data
	 */
	inline const T* getInternal()
	{
		return &data;
	}
	
	/** Get a copy of the internal data
	 * @param array the array to place the internal data into
	 */
	inline void getInternal(T* array)
	{
		for (int i=0; i < N; i++)
			array[i] = data[i];
	}
	
	/** Set the internal data for this vector
	 * @param array the array to get data from
	 */
	inline void setInternal(T* array)
	{
		for (int i=0; i < N; i++)
			data[i] = array[i];
	}
	
	
	/** Add a vector to this vector
	 * @param v1 the vector to add
	 */
	inline void add(const Vector<T,N>& v1)
	{	
		for (int i=0; i < N; i++)
			data[i] += v1.data[i];
	}
	
	/** Add two vectors together and store result in this vector
	 * @param v1 the first vector to add
	 * @param v2 the second vector to add
	 */
	inline void add(const Vector<T,N>& v1, const Vector<T,N>& v2)
	{
		for (int i=0; i < N; i++)
			data[i] = v1.data[i] + v2.data[i];
	}
	
	/** Subtract a vector from this vector
	 * @param v1 the vector to subtract
	 */
	inline void subtract(const Vector<T,N>& v1)
	{	
		for (int i=0; i < N; i++)
			data[i] -= v1.data[i];
	}
	
	/** Subtract one vector from another and store result in this vector
	 * @param v1 the first vector
	 * @param v2 the vector to subtract from the first
	 */
	inline void subtract(const Vector<T,N>& v1, const Vector<T,N>& v2)
	{
		for (int i=0; i < N; i++)
			data[i] = v1.data[i] - v2.data[i];
	}
	
	/** Scale this vector by a scalar
	 * @param scale the ammount to scale
	 */
	inline void scale(T scale)
	{	
		for (int i=0; i < N; i++)
			data[i] *= scale;
	}
	
	
};
	
	
/** Partial Template Specialization for Vectors
 * with two components
 */
template<class T>
class Vector<T, 2>
{
private:
	/// internal storage
	T data[2];
	
public:	
	/// default constructor, all components 0
	inline Vector()
	{
		data[0] = (T)0.0f;
		data[1] = (T)0.0f;
	} 
	
	/// standard constructor
	inline Vector(T x, T y)
	{
		data[0] = x;
		data[1] = y;
	}
	
	/// array constructor
	inline Vector(T* array)
	{
		for (int i=0; i < 2; i++)
			data[i] = array[i];
	}
	
	/// copy constructor
	inline Vector(const Vector<T,2>& copy)
	{
		memcpy(&data, &copy.data, sizeof(T)*2);
	}
	
	/** Get read-only access to internal data
	 * @return internal data
	 */
	inline const T* getInternal()
	{
		return &data;
	}
	
	/** Get a copy of the internal data
	 * @param array the array to place the internal data into
	 */
	inline void getInternal(T* array)
	{
		for (int i=0; i < 2; i++)
			array[i] = data[i];
	}
	
	/** Set the internal data for this vector
	 * @param array the array to get data from
	 */
	inline void setInternal(T* array)
	{
		for (int i=0; i < 2; i++)
			data[i] = array[i];
	}
	
	
	/** Add a vector to this vector
	 * @param v1 the vector to add
	 */
	inline void add(const Vector<T,2>& v1)
	{	
		for (int i=0; i < 2; i++)
			data[i] += v1.data[i];
	}
	
	/** Add two vectors together and store result in this vector
	 * @param v1 the first vector to add
	 * @param v2 the second vector to add
	 */
	inline void add(const Vector<T,2>& v1, const Vector<T,2>& v2)
	{
		for (int i=0; i < 2; i++)
			data[i] = v1.data[i] + v2.data[i];
	}
	
	/** Subtract a vector from this vector
	 * @param v1 the vector to subtract
	 */
	inline void subtract(const Vector<T,2>& v1)
	{	
		for (int i=0; i < 2; i++)
			data[i] -= v1.data[i];
	}
	
	/** Subtract one vector from another and store result in this vector
	 * @param v1 the first vector
	 * @param v2 the vector to subtract from the first
	 */
	inline void subtract(const Vector<T,2>& v1, const Vector<T,2>& v2)
	{
		for (int i=0; i < 2; i++)
			data[i] = v1.data[i] - v2.data[i];
	}
	
	/** Scale this vector by a scalar
	 * @param scale the ammount to scale
	 */
	inline void scale(T scale)
	{	
		for (int i=0; i < 2; i++)
			data[i] *= scale;
	}
	
};

// position forward declaration
class Position;


/** Partial Template Specialization for Vectors
 * with three components
 */
template<class T>
class Vector<T, 3>
{
private:
	/// internal storage
	T data[3];
	
public:	
	
	friend class Position;
	
	/// default constructor, all components 0
	inline Vector()
	{
		data[0] = (T)0.0f;
		data[1] = (T)0.0f;
		data[2] = (T)0.0f;
	} 
	
	/// standard constructor
	inline Vector(T x, T y, T z)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}
	
	/// array constructor
	inline Vector(T* array)
	{
		for (int i=0; i < 3; i++)
			data[i] = array[i];
	}
	
	/// copy constructor
	inline Vector(const Vector<T,3>& copy)
	{
		memcpy(&data, &copy.data, sizeof(T)*3);
	}
	
	/** Get read-only access to internal data
	 * @return internal data
	 */
	inline const T* getInternal()
	{
		return &data;
	}
	
	/** Get a copy of the internal data
	 * @param array the array to place the internal data into
	 */
	inline void getInternal(T* array)
	{
		for (int i=0; i < 3; i++)
			array[i] = data[i];
	}
	
	/** Set the internal data for this vector
	 * @param array the array to get data from
	 */
	inline void setInternal(T* array)
	{
		for (int i=0; i < 3; i++)
			data[i] = array[i];
	}
	
	/** Set the contents by copy
	 * @param copy the point to copy
	 */
	inline void set(const Vector<T,3>& copy)
	{
		memcpy(&data, &copy.data, sizeof(T)*3);
	}
	
	/** Set the contents by listing
	 * @param x the x to set
	 * @param y the y to set
	 * @param z the z to set
	 */
	inline void set(T x, T y, T z)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}
	
	// set components individually
	inline void setX(T x) { data[0] = x; }
	inline void setY(T y) { data[1] = y; }
	inline void setZ(T z) { data[2] = z; }
	
	// get components individually
	inline T getX() const { return data[0]; }
	inline T getY() const { return data[1]; }
	inline T getZ() const { return data[2]; }
	
	
	/** Add a vector to this vector
	 * @param v1 the vector to add
	 */
	inline void add(const Vector<T,3>& v1)
	{	
		for (int i=0; i < 3; i++)
			data[i] += v1.data[i];
	}
	
	/** Add two vectors together and store result in this vector
	 * @param v1 the first vector to add
	 * @param v2 the second vector to add
	 */
	inline void add(const Vector<T,3>& v1, const Vector<T,3>& v2)
	{
		for (int i=0; i < 3; i++)
			data[i] = v1.data[i] + v2.data[i];
	}
	
	/** Subtract a vector from this vector
	 * @param v1 the vector to subtract
	 */
	inline void subtract(const Vector<T,3>& v1)
	{	
		for (int i=0; i < 3; i++)
			data[i] -= v1.data[i];
	}
	
	/** Subtract one vector from another and store result in this vector
	 * @param v1 the first vector
	 * @param v2 the vector to subtract from the first
	 */
	inline void subtract(const Vector<T,3>& v1, const Vector<T,3>& v2)
	{
		for (int i=0; i < 3; i++)
			data[i] = v1.data[i] - v2.data[i];
	}
	
	/** Scale this vector by a scalar
	 * @param scale the ammount to scale
	 */
	inline void scale(T scale)
	{	
		for (int i=0; i < 3; i++)
			data[i] *= scale;
	}
	
	/** Cross product a vector to this vector
	 * @param v the vector to cross product with
	 */
	inline void crossProduct(const Vector<T,3>& v)
	{
		T tmp[3];
		tmp[0] = data[1]*v[2] - v[1]*data[2];
		tmp[1] = -data[0]*v[2] + v[0]*data[2];
		tmp[2] = data[0]*v[1] - v[0]*data[1];
		
		data[0] = tmp[0];
		data[1] = tmp[1];
		data[2] = tmp[2];
	}
	
	/** Cross product two vectors and store the result in this vector
	 * @param v1 the first vector to cross product with
	 * @param v2 the second vector to cross product with
	 */
	inline void crossProduct(const Vector<T,3>& v1, const Vector<T,3>& v2)
	{
		data[0] = v1.data[1]*v2.data[2] - v2.data[1]*v1.data[2];
		data[1] = -v1.data[0]*v2.data[2] + v2.data[0]*v1.data[2];
		data[2] = v1.data[0]*v2.data[1] - v2.data[0]*v1.data[1];
	}
	
	/** Dot product a vector and this vector
	 * @param v1 the vector to dot product with
	 * @return the dot product
	 */
	inline T dotProduct(const Vector<T,3>& v1)
	{	
		return (data[0]*v1.data[0]) + (data[1]*v1.data[1]) + (data[2]*v1.data[2]);
	}
	
	/** Get the angle between a vector and this vector
	 * @param v the vector to compare to
	 * @return angle between the two vectors
	 */
	inline T angleBetween(const Vector<T,3>& v)
	{
		T tmp = this->dotProduct(v);
		return T(acos(double(tmp)));
	}
	
	/** Get the magnitude/length of the vector
	 * @return length/magnitude of this vector
	 */
	inline T getLength()
	{
		return T(sqrt(double((data[0]*data[0])+(data[1]*data[1])+(data[2]*data[2]))));
	}
	
	/** Normalize this vector
	 */
	inline void normalize()
	{
		this->scale(T(1.0) / this->getLength());
	}
	
	/** Transform this vector using a matrix
	 * @param m the matrix to apply
	 */
	void transform(const Matrix<T,4,4>& m);
	
	/** Transform a vector using a matrix and store the result in this vector
	 * @param v the vector to use as a base
	 * @param m the matrix to apply
	 */
	void transform(const Vector<T,3>& v, const Matrix<T,4,4>& m);
	
	/** Perform only the rotation part of a transform
	 * @param m the matrix to apply rotation from
	 */
	void rotate(const Matrix<T,3,3>& m);
	
	/** Perform only the rotation part of a transform and store result in
	 * this vector
	 * @param v the vector to use as a base for the rotation
	 * @param m the matrix to apply rotation from
	 */
	void rotate(const Vector<T,3>& v, const Matrix<T,3,3>& m);
	
};


/** Partial Template Specialization for Vectors
 * with four components
 */
template<class T>
class Vector<T, 4>
{
private:
	/// internal storage
	T data[4];
	
public:	
	/// default constructor, all components 0
	inline Vector()
	{
		data[0] = (T)0.0f;
		data[1] = (T)0.0f;
		data[2] = (T)0.0f;
		data[3] = (T)1.0f; // w is almost always 1
	} 
	
	/// standard constructor
	inline Vector(T x, T y, T z, T w)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
	}
	
	/// array constructor
	inline Vector(T* array)
	{
		for (int i=0; i < 4; i++)
			data[i] = array[i];
	}
	
	/// copy constructor
	inline Vector(const Vector<T,4>& copy)
	{
		memcpy(&data, &copy.data, sizeof(T)*4);
	}
	
	/** Get read-only access to internal data
	 * @return internal data
	 */
	inline const T* getInternal()
	{
		return &data;
	}
	
	/** Get a copy of the internal data
	 * @param array the array to place the internal data into
	 */
	inline void getInternal(T* array)
	{
		for (int i=0; i < 4; i++)
			array[i] = data[i];
	}
	
	/** Set the internal data for this vector
	 * @param array the array to get data from
	 */
	inline void setInternal(T* array)
	{
		for (int i=0; i < 4; i++)
			data[i] = array[i];
	}
	
	
	/** Add a vector to this vector
	 * @param v1 the vector to add
	 */
	inline void add(const Vector<T,4>& v1)
	{	
		for (int i=0; i < 4; i++)
			data[i] += v1.data[i];
	}
	
	/** Add two vectors together and store result in this vector
	 * @param v1 the first vector to add
	 * @param v2 the second vector to add
	 */
	inline void add(const Vector<T,4>& v1, const Vector<T,4>& v2)
	{
		for (int i=0; i < 4; i++)
			data[i] = v1.data[i] + v2.data[i];
	}
	
	/** Subtract a vector from this vector
	 * @param v1 the vector to subtract
	 */
	inline void subtract(const Vector<T,4>& v1)
	{	
		for (int i=0; i < 4; i++)
			data[i] -= v1.data[i];
	}
	
	/** Subtract one vector from another and store result in this vector
	 * @param v1 the first vector
	 * @param v2 the vector to subtract from the first
	 */
	inline void subtract(const Vector<T,4>& v1, const Vector<T,4>& v2)
	{
		for (int i=0; i < 4; i++)
			data[i] = v1.data[i] - v2.data[i];
	}
	
	/** Scale this vector by a scalar
	 * @param scale the ammount to scale
	 */
	inline void scale(T scale)
	{	
		for (int i=0; i < 4; i++)
			data[i] *= scale;
	}
	
};




// popular vector formats
typedef Vector<float, 2> Vector2f;
typedef Vector<float, 3> Vector3f;
typedef Vector<float, 4> Vector4f;
typedef Vector<double, 2> Vector2d;
typedef Vector<double, 3> Vector3d;
typedef Vector<double, 4> Vector4d;
	
	
	
	
	
	
	
// operations that depend on Matrix
#include "Matrix.h"
	
	
/** Transform this vector using a matrix
 * @param m the matrix to apply
 */
template<class T>
void Vector<T,3>::transform(const Matrix<T,4,4>& m)
{
	T tmp[3];
	tmp[0] = m.data[0] * data[0] + m.data[4] * data[1] + m.data[8] *  data[2] + m.data[12]; 
	tmp[1] = m.data[1] * data[0] + m.data[5] * data[1] + m.data[9] *  data[2] + m.data[13];	
	tmp[2] = m.data[2] * data[0] + m.data[6] * data[1] + m.data[10] * data[2] + m.data[14];
	
	data[0] = tmp[0];
	data[1] = tmp[1];
	data[2] = tmp[2];
}

/** Transform a vector using a matrix and store the result in this vector
 * @param v the vector to use as a base
 * @param m the matrix to apply
 */
template<class T>
void Vector<T,3>::transform(const Vector<T,3>& v, const Matrix<T,4,4>& m)
{
	data[0] = m.data[0] * v.data[0] + m.data[4] * v.data[1] + m.data[8] *  v.data[2] + m.data[12]; 
	data[1] = m.data[1] * v.data[0] + m.data[5] * v.data[1] + m.data[9] *  v.data[2] + m.data[13];	
	data[2] = m.data[2] * v.data[0] + m.data[6] * v.data[1] + m.data[10] * v.data[2] + m.data[14];
}	

	
/** Perform only the rotation part of a transform
 * @param m the matrix to apply rotation from
 */
template<class T>
void Vector<T,3>::rotate(const Matrix<T,3,3>& m)
{
	T tmp[3];
	tmp[0] = m.data[0] * data[0] + m.data[3] * data[1] + m.data[6] * data[2];	
    tmp[1] = m.data[1] * data[0] + m.data[4] * data[1] + m.data[7] * data[2];	
    tmp[2] = m.data[2] * data[0] + m.data[5] * data[1] + m.data[8] * data[2];
	
	data[0] = tmp[0];
	data[1] = tmp[1];
	data[2] = tmp[2];
}
	
/** Perform only the rotation part of a transform and store result in
 * this vector
 * @param v the vector to use as a base for the rotation
 * @param m the matrix to apply rotation from
 */
template<class T>
void Vector<T,3>::rotate(const Vector<T,3>& v, const Matrix<T,3,3>& m)
{
	data[0] = m.data[0] * v.data[0] + m.data[3] * v.data[1] + m.data[6] * v.data[2];	
    data[1] = m.data[1] * v.data[0] + m.data[4] * v.data[1] + m.data[7] * v.data[2];	
    data[2] = m.data[2] * v.data[0] + m.data[5] * v.data[1] + m.data[8] * v.data[2];
}
	
	
	
	
};












#endif











