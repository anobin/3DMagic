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
/** Implementation file for Image class 
 *
 * @file Image.cpp
 * @author Andrew Keating
 */
 
#include <Graphics/Image.h>
#include <Util/StaticFont.h>


namespace Magic3D
{
    
    
/// destructor
Image::~Image()
{
    delete[] data;
}  
    

void Image::copyImage(Image* dest, const Image& source, int destX, 
    int destY, int sourceX, int sourceY, int width, int height )
{
    if (width < 0)
        width = source.width;
    if (height < 0)
        height = source.height;
    
    MAGIC_THROW( (sourceX+width) > source.width, "Width of rect too large.");
    MAGIC_THROW( (sourceY+height) > source.height, "Height of rect too large.");
    
    MAGIC_THROW( (destX+width) > dest->width, "Width of rect too large.");
    MAGIC_THROW( (destY+height) > dest->height, "Height of rect too large.");
    
    MAGIC_THROW( source.channels != dest->channels, "Channel counts do not match." );
    
    // have to copy row by row
    for(int row = 0; row < height; row++)
    {
        memcpy(&dest->data[destX*dest->channels + destY*dest->width*dest->channels + row*dest->width*dest->channels],
               &source.data[sourceX*source.channels + sourceY*source.width*source.channels + row*source.width*source.channels],
               width*dest->channels
               );
    }
}
   
    
void Image::drawAsciiText(const StaticFont& font, const char* str, int x, int y)
{
    for(int i=0; str[i]; i++)
    {
        const Character& c = font.getChar(str[i]);
        Image::copyImage(this, c.getBitmap().bitmap, x, y);
        x += c.getBitmap().bitmap.getWidth();
    }
}
    
    
    
    
    
};



















