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
/** Header file for the magic assertion
 *
 * @file magic_assert.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MAGIC_ASSERT_H
#define MAGIC3D_MAGIC_ASSERT_H


// If specified to do nothing on assert, then do nothing...
#if defined( MAGIC3D_ASSERT_DO_NOTHING )
#define MAGIC_ASSERT(x)


// If specified to throw an exception on an assert, then throw an exception...
#elif defined( MAGIC3D_ASSERT_DO_EXCEPTION )
#include "../Exceptions/MagicAssertException.h" 
#define MAGIC_ASSERT(x) \
    ( (x) ? : throw MagicAssertException(x) )
    
    
// Otherwise, print to standard IO and exit the application on assert
#else
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;
#define _MAGIC_ASSERT(x, file, line) \
    { if (!(x)) \
        {  cout << "(" << (file) << ":" << (line) << ") " << \
            "Assertion: " << #x << endl; \
           exit( 1 ); \
        } \
    }
#define MAGIC_ASSERT(x) _MAGIC_ASSERT((x), __FILE__, __LINE__)
#endif





#endif






