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
/** Header file for TTFontResource class
 *
 * @file TTFontResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_TRUE_TYPE_FONT_RESOURCE_H
#define MAGIC3D_TRUE_TYPE_FONT_RESOURCE_H

#include "../FontResource.h"
#include "../../Util/Character.h"

// include freetype
#include <ft2build.h>
#include FT_FREETYPE_H // yes it's a macro include and yes it's the standard way

namespace Magic3D
{

/** True Type font resource
 */
class TTFontResource : public FontResource
{
protected:
    FT_Face face;
		
public:
    /// standard constructor
    TTFontResource(const char* path, const std::string& name, ResourceManager& manager);
    
    
	/// destructor
	virtual ~TTFontResource();
	
	
	virtual void getChar(Character* c, unsigned int charcode, int width, int height) const;
	
	inline bool hasChar(unsigned int charcode) const
	{
	    return ( FT_Get_Char_Index( face, charcode ) != 0 );
	}
	
	inline int getCharCount() const
	{
	    return face->num_glyphs;
	}
	
	inline bool isScalable() const
	{
	    return (face->face_flags & FT_FACE_FLAG_SCALABLE);
	}
	
	inline int getFixedSizeCount() const
	{
	    return face->num_fixed_sizes;
	}

};


};



#endif







