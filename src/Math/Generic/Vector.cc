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

#include <Math/Generic/Vector.h>

#include <Math/Generic/Matrix4.h>
#include <Math/Generic/Matrix3.h>



Vector<3> Vector<3>::transform(const Matrix4& m) const
{
    Vector<3> v;
    v.data[0] = m.get(0,0) * x() + m.get(1,0) * y() + m.get(2,0) * z() + m.get(3,0); 
    v.data[1] = m.get(0,1) * x() + m.get(1,1) * y() + m.get(2,1) * z() + m.get(3,1); 
    v.data[2] = m.get(0,2) * x() + m.get(1,2) * y() + m.get(2,2) * z() + m.get(3,2);
	return v;
}


Vector<3> Vector<3>::rotate(const Matrix3& m) const
{
    Vector<3> v;
    v.data[0] = m.get(0,0) * x() + m.get(1,0) * y() + m.get(2,0) * z();   
    v.data[1] = m.get(0,1) * x() + m.get(1,1) * y() + m.get(2,1) * z();   
    v.data[2] = m.get(0,2) * x() + m.get(1,2) * y() + m.get(2,2) * z();
	return v;
}

Vector<4> Vector<4>::transform(const Matrix4 &m) const
{
    Vector<4> ret;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret.data[i] += m.get(i, j) * this->data[j];
        }
    }

    return ret;
}