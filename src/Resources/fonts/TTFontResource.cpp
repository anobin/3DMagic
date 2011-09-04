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
/** Implementation file for TTFontResource class
 *
 * @file TTFontResource.cpp
 * @author Andrew Keating
 */
 
#include <Resources/fonts/TTFontResource.h>

namespace Magic3D
{
    
extern int init_Freetype(FT_Library* library);
extern void quit_Freetype();
    
/// standard constructor
TTFontResource::TTFontResource(const char* path, const std::string& name, ResourceManager& manager):
    FontResource(name, manager), face(NULL)
{
    // we need freetype library
    FT_Library library;
    int error = init_Freetype(&library);
    if (error)
        throw_MagicException("Failed to initalize freetype library");
    
    // create face for this font
    error = FT_New_Face(library, path,
	    0, // only want face index 0, some fonts have more than 1 index 
	    &this->face );
	if ( error == FT_Err_Unknown_File_Format ) 
	{
	    quit_Freetype();
	    throw_MagicException("Font format unsupported.");
	}
	else if (error)
	{
	    quit_Freetype();
	    throw_MagicException("Failed to open font file." );
	}
}
    
/// destructor
TTFontResource::~TTFontResource()
{
    // we're done with the face
    FT_Done_Face(this->face);
    
    // we're done with freetype library
    quit_Freetype();
}

bool TTFontResource::hasChar(unsigned int charcode) const
{
    return ( FT_Get_Char_Index( face, charcode ) != 0 );
}

void TTFontResource::getGlyph(Character* c, int glyphIndex, int width, int height) const
{
    // set width and height, if they have changed
    int error = FT_Set_Pixel_Sizes(this->face, width, height );
	if (error)
	    throw_MagicException( "Failed to set character size for font.");
	
	error = FT_Load_Glyph( this->face, glyphIndex, FT_LOAD_DEFAULT);
	if (error)
	    throw_MagicException("Failed to load character glyph.");
	
	// if the glyph was a scalable format and not a bitmap, convert to bitmap
	if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
	{
	    error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
	    if (error)
	        throw_MagicException("Failed to convert glyph to bitmap.");
	}
	
	// set metrics for character, freetype metrics are stored as 1/64th of a pixel
	FT_Glyph_Metrics&  metrics = face->glyph->metrics;
	Character::Metrics& m = c->getMetrics();
	m.width = ((float)metrics.width) / 64.0f;
	m.height = ((float)metrics.height) / 64.0f;
	m.horiBearingX = ((float)metrics.horiBearingX) / 64.0f;
	m.horiBearingY = ((float)metrics.horiBearingY) / 64.0f;
	m.horiAdvance = ((float)metrics.horiAdvance) / 64.0f;
	if (FT_HAS_VERTICAL(this->face))
	{
	    m.vertBearingX = ((float)metrics.vertBearingX) / 64.0f;
	    m.vertBearingY = ((float)metrics.vertBearingY) / 64.0f;
	    m.vertAdvance = ((float)metrics.vertAdvance) / 64.0f;
	}
	else
	{
	    m.vertBearingX = 0.0f;
	    m.vertBearingY = 0.0f;
	    m.vertAdvance = 0.0f;
	}
	
	// set bitmap left and right for char
	FT_Bitmap& bitmap = face->glyph->bitmap;
	Character::Bitmap& b = c->getBitmap();
	b.bitmap_left = face->glyph->bitmap_left;
	b.bitmap_top = face->glyph->bitmap_top;
	
	// copy bitmap data, expanding out to RGB
	b.bitmap.allocate(bitmap.width, bitmap.rows, 3); // 3 channel RGB
	unsigned char* charData = b.bitmap.getMutableRawData();
	unsigned char* bt = bitmap.buffer;
	if (bitmap.pixel_mode != FT_PIXEL_MODE_GRAY	)
	    throw_MagicException("font glyph bitmap is in wrong format." );
	for (int y=0; y < bitmap.rows; y++)
	{
	    for (int x=0; x < bitmap.width; x++)
	    {
	        charData[(y*bitmap.width+x)*3 + 0] = bt[y*bitmap.pitch + x]; // RED
	        charData[(y*bitmap.width+x)*3 + 1] = bt[y*bitmap.pitch + x]; // GREEN
	        charData[(y*bitmap.width+x)*3 + 2] = bt[y*bitmap.pitch + x]; // BLUE
	    }
	}
}
    
    
void TTFontResource::getChar(Character* c, unsigned int charcode, int width,
    int height ) const
{
    this->getGlyph(c, FT_Get_Char_Index( face, charcode ), width, height); 
    // set charcode for character
	c->setCharCode(charcode);
}   
    
void TTFontResource::getMissingChar(Character* c, int width, int height) const
{
    this->getGlyph(c, 0, width, height); // glyph index 0 is for missing char
    // set charcode for character
	c->setCharCode(0); // really shouldn't matter for missing char
}
    
    
    
    
};

























