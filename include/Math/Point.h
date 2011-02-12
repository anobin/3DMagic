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
/** Header file for the Point class
 *
 * @file Point.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POINT_H
#define MAGIC3D_POINT_H

#include "Matrix.h"
#include "../Exceptions/MagicException.h"
#include <string.h>
#include <math.h>


namespace Magic3D
{

	
/** Represents a single point with any number
 * of components, the most popular formats are 
 * typedef'ed
 */
template <class T, int N>
class Point
{
private:
	/// internal storage
	T data[N];
	
public:	
	
	/// default constructor, all components 0
	inline Point()
	{
		for (int i=0; i < N; i++)
			data[i] = (T)0.0f; // most likey a float type
	} 
	
	/// array constructor
	inline Point(T* array)
	{
		for (int i=0; i < N; i++)
			data[i] = array[i];
	}
	
	/// copy constructor
	inline Point(const Point<T,N>& copy)
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
	
	/** Set the internal data
	 * @param array the array to get internal data from
	 */
	inline void setInternal(const T* array)
	{
		for (int i=0; i < N; i++)
			data[i] = array[i];
	}
	
};
	
	
/** Partial Template Specialization for Points
 * with two components
 */
template<class T>
class Point<T, 2>
{
private:
	/// internal storage
	T data[2];
	
public:	
	/// default constructor, all components 0
	inline Point()
	{
		data[0] = (T)0.0f;
		data[1] = (T)0.0f;
	} 
	
	/// standard constructor
	inline Point(T x, T y)
	{
		data[0] = x;
		data[1] = y;
	}
	
	/// array constructor
	inline Point(T* array)
	{
		for (int i=0; i < 2; i++)
			data[i] = array[i];
	}
	
	/// copy constructor
	inline Point(const Point<T,2>& copy)
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
	
	inline T getX() const { return data[0]; }
	inline T getY() const { return data[1]; }
	
	/** Get a copy of the internal data
	 * @param array the array to place the internal data into
	 */
	inline void getInternal(T* array)
	{
		for (int i=0; i < 2; i++)
			array[i] = data[i];
	}
	
	/** Set the internal data
	 * @param array the array to get internal data from
	 */
	inline void setInternal(const T* array)
	{
		for (int i=0; i < 2; i++)
			data[i] = array[i];
	}
	
};


/** Partial Template Specialization for Points
 * with three components
 */
template<class T>
class Point<T, 3>
{
private:
	/// internal storage
	T data[3];
	
public:	
	/// default constructor, all components 0
	inline Point()
	{
		data[0] = (T)0.0f;
		data[1] = (T)0.0f;
		data[2] = (T)0.0f;
	} 
	
	/// standard constructor
	inline Point(T x, T y, T z)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}
	
	/// array constructor
	inline Point(T* array)
	{
		for (int i=0; i < 3; i++)
			data[i] = array[i];
	}
	
	/// copy constructor
	inline Point(const Point<T,3>& copy)
	{
		memcpy(&data, &copy.data, sizeof(T)*3);
	}
	
	/** Get read-only access to internal data
	 * @return internal data
	 */
	inline const T* getInternal()
	{
		return (T*)&data;
	}
	
	/** Get a copy of the internal data
	 * @param array the array to place the internal data into
	 */
	inline void getInternal(T* array)
	{
		for (int i=0; i < 3; i++)
			array[i] = data[i];
	}
	
	/** Set the internal data
	 * @param array the array to get internal data from
	 */
	inline void setInternal(const T* array)
	{
		for (int i=0; i < 3; i++)
			data[i] = array[i];
	}
	
	/** Set the contents by copy
	 * @param copy the point to copy
	 */
	inline void set(const Point<T,3>& copy)
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
	
	/** Translate by the specified ammounts
	 * @param x the x ammount to translate
	 * @param y the y ammount to translate
	 * @param z the z ammount to translate
	 */
	inline void translate(T x, T y, T z)
	{
		data[0] += x;
		data[1] += y;
		data[2] += z;
	}
	
	/** Get the distance to another point
	 * @param p the point to compare to
	 * @return the distance between
	 */
	inline T getDistance(const Point<T,3>& p)
	{
		T x = data[0] - p.data[0];
		x = x*x;
	
		T y = data[1] - p.data[1];
		y = y*y;

		T z = data[2] - p.data[2];
		z = z*z;

		return T(sqrt(double(x + y + z)));
	}
	
	/** Transform this point using a matrix
	 * @param m the matrix to apply
	 */
	inline void transform(const Matrix<T,4,4>& m)
	{
		T tmp[3];
		tmp[0] = m.data[0] * data[0] + m.data[4] * data[1] + m.data[8] *  data[2] + m.data[12]; 
		tmp[1] = m.data[1] * data[0] + m.data[5] * data[1] + m.data[9] *  data[2] + m.data[13];	
		tmp[2] = m.data[2] * data[0] + m.data[6] * data[1] + m.data[10] * data[2] + m.data[14];
		
		data[0] = tmp[0];
		data[1] = tmp[1];
		data[2] = tmp[2];
	}
	
	/** Transform a point using a matrix and store the result in this point
	 * @param v the vector to use as a base
	 * @param m the matrix to apply
	 */
	inline void transform(const Vector<T,3>& v, const Matrix<T,4,4>& m)
	{
		data[0] = m.data[0] * v.data[0] + m.data[4] * v.data[1] + m.data[8] *  v.data[2] + m.data[12]; 
		data[1] = m.data[1] * v.data[0] + m.data[5] * v.data[1] + m.data[9] *  v.data[2] + m.data[13];	
		data[2] = m.data[2] * v.data[0] + m.data[6] * v.data[1] + m.data[10] * v.data[2] + m.data[14];	
	}
	
	
};


/** Partial Template Specialization for Points
 * with four components
 */
template<class T>
class Point<T, 4>
{
private:
	/// internal storage
	T data[4];
	
public:	
	/// default constructor, all components 0
	inline Point()
	{
		data[0] = (T)0.0f;
		data[1] = (T)0.0f;
		data[2] = (T)0.0f;
		data[3] = (T)1.0f; // w is almost always 1
	} 
	
	/// standard constructor
	inline Point(T x, T y, T z, T w)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
	}
	
	/// array constructor
	inline Point(T* array)
	{
		for (int i=0; i < 4; i++)
			data[i] = array[i];
	}
	
	/// copy constructor
	inline Point(const Point<T,4>& copy)
	{
		memcpy(&data, &copy.data, sizeof(T)*4);
	}
	
	/** Get read-only access to internal data
	 * @return internal data
	 */
	inline const T* getInternal()
	{
		return (T*)&data;
	}
	
	/** Get a copy of the internal data
	 * @param array the array to place the internal data into
	 */
	inline void getInternal(T* array)
	{
		for (int i=0; i < 4; i++)
			array[i] = data[i];
	}
	
	/** Set the internal data
	 * @param array the array to get internal data from
	 */
	inline void setInternal(const T* array)
	{
		for (int i=0; i < 4; i++)
			data[i] = array[i];
	}
	
	/** Set the contents by copy
	 * @param copy the point to copy
	 */
	inline void set(const Point<T,4>& copy)
	{
		memcpy(&data, &copy.data, sizeof(T)*4);
	}
	
	/** Set the contents by listing
	 * @param x the x to set
	 * @param y the y to set
	 * @param z the z to set
	 */
	inline void set(T x, T y, T z, T w)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
	}
	
	// set components individually
	inline void setX(T x) { data[0] = x; }
	inline void setY(T y) { data[1] = y; }
	inline void setZ(T z) { data[2] = z; }
	inline void setW(T w) { data[3] = w; }
	
	// get components individually
	inline T getX() const { return data[0]; }
	inline T getY() const { return data[1]; }
	inline T getZ() const { return data[2]; }
	inline T getW() const { return data[3]; }
	
	/** Transform this point using a matrix
	 * @param m the matrix to apply
	 */
	inline void transform(const Matrix<T,4,4>& m)
	{
		T tmp[4];
		tmp[0] = m.data[0] * data[0] + m.data[4] * data[1] + m.data[8] *  data[2] + m.data[12] * data[3];	 
		tmp[1] = m.data[1] * data[0] + m.data[5] * data[1] + m.data[9] *  data[2] + m.data[13] * data[3];	
		tmp[2] = m.data[2] * data[0] + m.data[6] * data[1] + m.data[10] * data[2] + m.data[14] * data[3];	
		tmp[3] = m.data[3] * data[0] + m.data[7] * data[1] + m.data[11] * data[2] + m.data[15] * data[3];
		
		data[0] = tmp[0];
		data[1] = tmp[1];
		data[2] = tmp[2];
		data[3] = tmp[3];
	}
	
	/** Transform a point using a matrix and store the result in this vector
	 * @param v the vector to use as a base
	 * @param m the matrix to apply
	 */
	inline void transform(const Vector<T,4>& v, const Matrix<T,4,4>& m)
	{
		data[0] = m.data[0] * v.data[0] + m.data[4] * v.data[1] + m.data[8] *  v.data[2] + m.data[12] * v.data[3];	 
		data[1] = m.data[1] * v.data[0] + m.data[5] * v.data[1] + m.data[9] *  v.data[2] + m.data[13] * v.data[3];	
		data[2] = m.data[2] * v.data[0] + m.data[6] * v.data[1] + m.data[10] * v.data[2] + m.data[14] * v.data[3];	
		data[3] = m.data[3] * v.data[0] + m.data[7] * v.data[1] + m.data[11] * v.data[2] + m.data[15] * v.data[3];
	}
};




// popular vector formats
typedef Point<float, 2> Point2f;
typedef Point<float, 3> Point3f;
typedef Point<float, 4> Point4f;
typedef Point<double, 2> Point2d;
typedef Point<double, 3> Point3d;
typedef Point<double, 4> Point4d;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};












#endif











