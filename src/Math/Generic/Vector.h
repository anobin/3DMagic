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
#ifndef MAGIC3D_VECTOR_GENERIC_H
#define MAGIC3D_VECTOR_GENERIC_H

// for Scalar
#include "MathTypes.h"

#include "BaseVector.h"

class Matrix3;
class Matrix4;

template<int size>
class Vector : public BaseVector<size, Vector<size>>
{    
public:
	inline Vector() {}

	inline Vector(const Vector<size>& copy): BaseVector(copy) {}

	inline Vector(const Scalar data[size]): BaseVector(data) {}
};

template<>
class Vector<2> : public BaseVector<2, Vector<2>>
{    
public:
	inline Vector() {}

	inline Vector(const Vector<2>& copy): BaseVector(copy) {}

	inline Vector(const Scalar data[2]): BaseVector(data) {}

	inline Vector(Scalar x, Scalar y)
	{
		data[0] = x;
		data[1] = y;
	}

	inline Scalar x() const { return data[0]; }
	inline Vector<2> withX(Scalar x) const { return with(0, x); }
	inline Scalar y() const { return data[1]; }
	inline Vector<2> withY(Scalar y) const { return with(1, y); }

};
typedef Vector<2> Vector2;

template<>
class Vector<3> : public BaseVector<3, Vector<3>>
{    
public:
	inline Vector() {}

	inline Vector(const Vector<3>& copy): BaseVector(copy) {}

	inline Vector(const Scalar data[3]): BaseVector(data) {}

	inline Vector(Scalar x, Scalar y, Scalar z)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}

	inline Scalar x() const { return data[0]; }
	inline Vector<3> withX(Scalar x) const { return with(0, x); }
	inline Scalar y() const { return data[1]; }
	inline Vector<3> withY(Scalar y) const { return with(1, y); }
	inline Scalar z() const { return data[2]; }
	inline Vector<3> withZ(Scalar z) const { return with(2, z); }

    using BaseVector::operator*;

    inline Vector<3> crossProduct(const Vector<3> &v) const
    {
        return Vector<3>(
			y()*v.z() - v.y()*z(),
			-x()*v.z() + v.x()*z(),
			x()*v.y() - v.x()*y()
		);
    }
    inline Vector<3> operator*(const Vector<3>& v) const
    {
        return this->crossProduct(v);
    }

	Vector<3> transform(const Matrix4& m) const;

    Vector<3> rotate(const Matrix3& m) const;
};
typedef Vector<3> Vector3;

template<>
class Vector<4> : public BaseVector<4, Vector<4>>
{    
public:
	inline Vector() {}

	inline Vector(const Vector<4>& copy): BaseVector(copy) {}

	inline Vector(const Scalar data[4]): BaseVector(data) {}

	inline Vector(Scalar x, Scalar y, Scalar z, Scalar w)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
	}

	inline Scalar x() const { return data[0]; }
	inline Vector<4> withX(Scalar x) const { return with(0, x); }
	inline Scalar y() const { return data[1]; }
	inline Vector<4> withY(Scalar y) const { return with(1, y); }
	inline Scalar z() const { return data[2]; }
	inline Vector<4> withZ(Scalar z) const { return with(2, z); }
	inline Scalar w() const { return data[3]; }
	inline Vector<4> withW(Scalar w) const { return with(3, w); }
};
typedef Vector<4> Vector4;

#endif