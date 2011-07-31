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
/** Header file for the magic exception throws, that
 * is exception throws that can be turned off. Note
 * that when magic throws are disabled, then they will
 * do nothing at all.
 *
 * @file magic_throw.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MAGIC_THROW_H
#define MAGIC3D_MAGIC_THROW_H


// If magic throws are disabled, then they do nothing at all
#if defined( MAGIC3D_DISABLE_MAGIC_THROWS )
#define MAGIC_THROW_EX(cond, ex, msg) {}
#define MAGIC_THROW(cond, msg) {}


// If magic throws are enabled, then throw if condition is true (opposite of exceptions)
#else 
#include "../Exceptions/MagicException.h"

#define MAGIC_THROW_EX(cond, ex, msg) \
    { if (cond) { throw ex(msg); } }
// generic version that always throws base exception class (MagicException)
#define MAGIC_THROW(cond, msg) \
    { if (cond) { throw MagicException(msg); } }
    
#endif





#endif




































