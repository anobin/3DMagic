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
/** Selector file for type definitions for the Math interface
 *
 * @file MathTypes.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MATH_TYPES_SELECTOR_H
#define MAGIC3D_MATH_TYPES_SELECTOR_H


// include the actual math types based on config

// generic (portable) implementation
#ifdef M3D_MATH_USE_GENERIC
#include "Generic/MathTypes.h"


// intel processors only implementation
#elif defined(M3D_MATH_USE_INTEL)
#include "Intel/MathTypes.h"


// nothing is selected, not valid for math interface
#else
#error "No Math Implementation is selected"

/* the following is provided as documentation of the types that need to
 * be defined for the Math interface 
 */

/// scalar floating-point base type
#ifdef M3D_MATH_DOUBLE_PERCISION
typedef Scalar double;
#else
typedef Scalar float;
#endif





#endif // end of selector branch

#endif



