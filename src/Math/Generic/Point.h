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
#ifndef MAGIC3D_POINT_GENERIC_H
#define MAGIC3D_POINT_GENERIC_H

// for Scalar
#include "MathTypes.h"

#include "BasePoint.h"

class Matrix3;
class Matrix4;

template<int size>
class Point : public BasePoint<size, Point<size>>
{    
public:
	inline Point() {}

	inline Point(const Point<size>& copy): BasePoint(copy) {}

	inline Point(const Scalar data[size]): BasePoint(data) {}
};

template<>
class Point<2> : public BasePoint<2, Point<2>>
{    
public:
	inline Point() {}

	inline Point(const Point<2>& copy): BasePoint(copy) {}

	inline Point(const Scalar data[2]): BasePoint(data) {}

	inline Point(Scalar x, Scalar y)
	{
		data[0] = x;
		data[1] = y;
	}

	inline Scalar x() const { return data[0]; }
	inline Point<2> withX(Scalar x) const { return with(0, x); }
	inline Scalar y() const { return data[1]; }
	inline Point<2> withY(Scalar y) const { return with(1, y); }

};
typedef Point<2> Point2;

template<>
class Point<3> : public BasePoint<3, Point<3>>
{    
public:
	inline Point() {}

	inline Point(const Point<3>& copy): BasePoint(copy) {}

	inline Point(const Scalar data[3]): BasePoint(data) {}

	inline Point(Scalar x, Scalar y, Scalar z)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}

	inline Scalar x() const { return data[0]; }
	inline Point<3> withX(Scalar x) const { return with(0, x); }
	inline Scalar y() const { return data[1]; }
	inline Point<3> withY(Scalar y) const { return with(1, y); }
	inline Scalar z() const { return data[2]; }
	inline Point<3> withZ(Scalar z) const { return with(2, z); }

	inline bool isAtOrigin()
	{
		return (x()==0) && (y()==0) && (z()==0);
	}

	using BasePoint::translate;

	inline Point<3> translate(Scalar x, Scalar y, Scalar z) const
	{
		return Point<3>(
			this->x() + x,
			this->y() + y,
			this->z() + z
		);
	}

	Point<3> transform(const Matrix4& m) const;

	Point<3> rotate(const Matrix3& m) const;
};
typedef Point<3> Point3;

template<>
class Point<4> : public BasePoint<4, Point<4>>
{    
public:
	inline Point() {}

	inline Point(const Point<4>& copy): BasePoint(copy) {}

	inline Point(const Scalar data[4]): BasePoint(data) {}

	inline Point(Scalar x, Scalar y, Scalar z, Scalar w)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
	}

	inline Scalar x() const { return data[0]; }
	inline Point<4> withX(Scalar x) const { return with(0, x); }
	inline Scalar y() const { return data[1]; }
	inline Point<4> withY(Scalar y) const { return with(1, y); }
	inline Scalar z() const { return data[2]; }
	inline Point<4> withZ(Scalar z) const { return with(2, z); }
	inline Scalar w() const { return data[3]; }
	inline Point<4> withW(Scalar w) const { return with(3, w); }

	Point<4> transform(const Matrix4& m) const;
};
typedef Point<4> Point4;

#endif