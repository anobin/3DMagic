/* 
Copyright (c) 2015 Andrew Keating

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
#ifndef MAGIC3D_BASE_POINT_GENERIC_H
#define MAGIC3D_BASE_POINT_GENERIC_H

// for Scalar
#include "MathTypes.h"

// for cos, sin, and tan
#include <math.h>

#include "Vector.h"

template<int size, typename T>
class BasePoint
{
protected:
    Scalar data[size];

	inline BasePoint() 
	{
		for (int i=0; i < size; i++)
			data[i] = 0;
	}

	inline BasePoint(const BasePoint<size,T>& copy)
	{
		for (int i=0; i < size; i++)
			data[i] = copy.data[i];
	}

	inline BasePoint(const Scalar data[size])
	{
		for (int i=0; i < size; i++)
			this->data[i] = data[i];
	}
    
public:

    inline Scalar operator[](int component) const
	{
		return data[component];
	}

    inline T translate(const Vector<size>& direction, Scalar distance) const
	{
		T newPoint;
		for (int i=0; i < size; i++)
			newPoint.data[i] = data[i] + (direction[i] * distance);
		return newPoint;
	}

	inline Scalar distanceTo(const T& p) const
	{
		Scalar distance = 0;
		for (int i=0; i < size; i++)
			distance += pow(data[i] - p[i], 2);
		return sqrt(distance);
	}

	inline Vector<size> vectorTowards(const T& p) const
	{
		Scalar newData[size];
		for(int i=0; i < size; i++)
			newData[size] = p[i] - data[i];
		return Vector<size>(newData);
	}

	inline T with(int component, Scalar value) const
	{
		T newV;
		for (int i=0; i < size; i++)
			newV.data[i] = data[i];
		newV.data[component] = value;
		return newV;
	}
};



#endif