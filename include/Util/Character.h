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
/** Header file for Character class
 *
 * @file Character.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_CHARACTER_H
#define MAGIC3D_CHARACTER_H

#include "../Exceptions/MagicException.h"
#include "magic_throw.h"
#include "../Graphics/Image.h"

// everything is in 3DMagic namespace
namespace Magic3D
{
    
	
/** Represents a single character in a font, has a set size
 */
class Character
{
public:
    struct Metrics
    {
        float width;    // same as bitmap.width
        float height;   // same as bitmap.height
        
        float horiBearingX; // x bearing of horizontal char in flaot px
        float horiBearingY; // y bearing of horizontal char in float px
        float horiAdvance;  // advance of horizontal char in float px
        
        float vertBearingX;
        float vertBearingY;
        float vertAdvance;
        
        inline Metrics(): width(0), height(0), horiBearingX(0), horiBearingY(0),
        horiAdvance(0), vertBearingX(0), vertAdvance(0) {}
    };
    
    struct Bitmap
    {
        Image bitmap;
	
        int bitmap_left;
        int bitmap_top;
        
        inline Bitmap(): bitmap_left(0), bitmap_top(0) {}
    };
    
private:
	Metrics metrics;
	
	Bitmap bitmap;
	
	int charcode;
	
public:	
	/// standard constructor for empty character
	inline Character(): charcode(0) {}
	
	virtual ~Character();
	
	inline Metrics& getMetrics()
	{
	    return metrics;
	}
	
	inline Bitmap& getBitmap()
	{
	    return bitmap;
	}
	
	inline const Metrics& getMetrics() const
	{
	    return metrics;
	}
	
	inline const Bitmap& getBitmap() const
	{
	    return bitmap;
	}
	
	inline int getCharCode() const
	{
	    return charcode;
	}
	
	inline void setCharCode(int charcode)
	{
	    this->charcode = charcode;
	}
	
};
	
	
	
	
	
	
	
	
	
};


#endif











