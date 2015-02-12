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
#ifndef MAGIC3D_BASE_VECTOR_GENERIC_H
#define MAGIC3D_BASE_VECTOR_GENERIC_H

// for Scalar
#include "MathTypes.h"

// for cos, sin, and tan
#include <math.h>

template<int size, typename T>
class BaseVector
{
protected:
    Scalar data[size];

	inline BaseVector() 
	{
		for (int i=0; i < size; i++)
			data[i] = 0;
	}

	inline BaseVector(const BaseVector<size,T>& copy)
	{
		for (int i=0; i < size; i++)
			data[i] = copy.data[i];
	}

	inline BaseVector(const Scalar data[size])
	{
		for (int i=0; i < size; i++)
			this->data[i] = data[i];
	}
    
public:

    inline Scalar operator[](int component) const
	{
		return data[component];
	}

    inline T add(const T& v) const
    {
        T ret;
		for(int i=0; i < size; i++)
			ret.data[i] = data[i] + v.data[i];
		return ret;
    }
    inline T operator+(const T& v) const
    {
        return this->add(v);
    }

    /// subtract a vector from this vector
    inline T subtract(const T& v) const 
    {
        T ret;
		for(int i=0; i < size; i++)
			ret.data[i] = data[i] - v.data[i];
		return ret;
    }
    inline T operator-(const T& v) const
    {
        return this->subtract(v);
    }

    /// scale this vector by a given factor
    inline T scale(Scalar factor) const
    {
        T ret;
		for(int i=0; i < size; i++)
			ret.data[i] = data[i] * factor;
		return ret;
    }
    inline T operator*(Scalar factor) const
    {
        return this->scale(factor);
    }

    /// dot product another vector with this vector
    inline Scalar dotProduct(const T& v) const
    {
		Scalar product = 0;
		for(int i=0; i < size; i++)
			product += (data[i] * v.data[i]);
		return product;
    }

    /// find the angle between this vector and another
    inline Scalar angleBetween(const T& v) const
    {
        return acos(this->dotProduct(v));
    }

    /// get the length of this vector
    inline Scalar getLength() const
    {
		Scalar product = 0;
		for(int i=0; i < size; i++)
			product += (data[i] * data[i]);
		return sqrt(product);
    }

    /// normalize this vector (turn into unit vector)
    inline T normalize() const
    {
        return this->scale(Scalar(1.0) / this->getLength());
    }

	inline T with(int component, Scalar value) const
	{
		T newV;
		for (int i=0; i < size; i++)
			newV.data[i] = data[i];
		newV.data[component] = value;
		return newV;
	}

    inline Scalar distanceTo(const T& v) const
    {
        Scalar distance = 0;
        for (int i = 0; i < size; i++)
            distance += pow(data[i] - v[i], 2);
        return sqrt(distance);
    }
};



#endif