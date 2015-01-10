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
/** Implementation file for Point3 Generic implementation
 *
 * @file Point3.cc
 * @author Andrew Keating
 */

#include <Math/Generic/Matrix4.h>
#include <Math/Generic/Matrix3.h>
#include <Math/Generic/Point4.h>




/// transform this point using a matrix
Point4 Point4::transform(const Matrix4 &m) const
{
    Point4 ret;
	ret.w = 0;

	for(int i=0; i < 4; i++)
	{
		for(int j=0; j < 4; j++)
		{
			ret.set(i, ret.get(i) + (m.get(i, j) * this->get(j)) );
		}
	}

	return ret;
}






































