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

#include <Math/Generic/Matrix4.h>
#include <Math/Generic/Matrix3.h>
#include <Math/Generic/Point.h>


Point<3> Point<3>::transform(const Matrix4 &m) const
{
    Point<3> newPoint;
    newPoint.data[0] = m.get(0,0) * x() + m.get(1,0) * y() + m.get(2,0) * z() + m.get(3,0); 
    newPoint.data[1] = m.get(0,1) * x() + m.get(1,1) * y() + m.get(2,1) * z() + m.get(3,1); 
    newPoint.data[2] = m.get(0,2) * x() + m.get(1,2) * y() + m.get(2,2) * z() + m.get(3,2);
    return newPoint;
}

Point<3> Point<3>::rotate(const Matrix3 &m) const
{
    Point<3> newPoint;
    newPoint.data[0] = m.get(0,0) * x() + m.get(1,0) * y() + m.get(2,0) * z();   
    newPoint.data[1] = m.get(0,1) * x() + m.get(1,1) * y() + m.get(2,1) * z();   
    newPoint.data[2] = m.get(0,2) * x() + m.get(1,2) * y() + m.get(2,2) * z();
    return newPoint;
}

Point<4> Point<4>::transform(const Matrix4 &m) const
{
    Point<4> ret;

	for(int i=0; i < 4; i++)
	{
		for(int j=0; j < 4; j++)
		{
			ret.data[i] += m.get(i, j) * this->data[j];
		}
	}

	return ret;
}