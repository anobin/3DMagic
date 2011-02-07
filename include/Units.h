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
/** Contains Unit definitions to help
 * establish a common measurement system that
 * can be easily scaled.
 *
 * @file Units.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_UNITS_H
#define MAGIC3D_UNITS_H

// everything is in 3DMagic namespace
namespace Magic3D
{

	
// meter is set to be 1.0f to ease compatable with Bullet physics library
static const float METER = 1.0f;
static const float FOOT = METER/3.28f;
static const float INCH = (FOOT/12.0f);
static const float CENTIMETER = (METER/100.f);
static const float MILIMETER = (METER/1000.0f);

















};

#endif

