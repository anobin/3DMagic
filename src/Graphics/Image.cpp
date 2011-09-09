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

void Image::blendImage(Image* dest, const Image& source, int destX,
    int destY, int sourceX, int sourceY, int width, int height )
{
    // ensure each image is RGBA
    MAGIC_THROW(dest->channels != 4, "Destination image for blend must be RGBA." );
    MAGIC_THROW(source.channels != 4, "Source image for blend must be RGBA." );
    
    if (width < 0)
        width = source.width;
    if (height < 0)
        height = source.height;
    
    MAGIC_THROW( (sourceX+width) > source.width, "Width of rect too large.");
    MAGIC_THROW( (sourceY+height) > source.height, "Height of rect too large.");
    
    MAGIC_THROW( (destX+width) > dest->width, "Width of rect too large.");
    MAGIC_THROW( (destY+height) > dest->height, "Height of rect too large.");
    
    // don't let unsigned char overflow, if we go over max, we can clamp to max
#define prevent_overflow(x) ( (x) > 255.0f ? 255 : (x) ) 
    
    // have to blend pixel by pixel
    // this whole algorithm can and should be optimized, this is the simple readable implementation
    // should probably also expand this function to be able to do different blend modes,
    //     right now we are replicaiting glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    unsigned char* d_row = &dest->data[destX*dest->channels + destY*dest->width*dest->channels];
    const unsigned char* s_row = &source.data[sourceX*source.channels + sourceY*source.width*source.channels];
    float sa;
    float da;
    for(int row = 0; row < height; row++)
    {
        unsigned char* d = d_row + (row*dest->width*dest->channels);
        const unsigned char* s = s_row + (row*source.width*source.channels);
        for(int col=0; col < width; col++)
        {
            // get scale factors for pixel
            sa = RGB_byte2float(s[3]);
            da = 1.0f - sa;
            
            // blend pixel
            d[0] = prevent_overflow( s[0]*sa + d[0]*da );
            d[1] = prevent_overflow( s[1]*sa + d[1]*da );
            d[2] = prevent_overflow( s[2]*sa + d[2]*da );
            d[3] = prevent_overflow( s[3]*sa + d[3]*da );
            
            // move to next pixel in row
            d += dest->channels;
            s += source.channels;
        }
    }
    
#undef prevent_overflow

}
   
    
void Image::drawAsciiText(const StaticFont& font, const char* str, int x, int y, const Color& color)
{
    // TODO: add support for adding text to any channel size
    for(int i=0; str[i]; i++)
    {
        // create image with color parameter and alpha channel from character
        const Character& c = font.getChar(str[i]);
        MAGIC_ASSERT(c.getBitmap().bitmap.channels == 1); // character bitmap should just be alpha
        Image m(c.getBitmap().bitmap.width, c.getBitmap().bitmap.height, 4);
        m.clear(color); 
        // TODO: add method to copy a one channel image into a channel in a multi-channel image
        for(int i=0; i < c.getBitmap().bitmap.width*c.getBitmap().bitmap.height; i++)
            m.data[i*4+3] = c.getBitmap().bitmap.data[i];
        Image::blendImage(this, m, x, y);
        x += c.getBitmap().bitmap.getWidth();
    }
}
    
    
    
    
    
};



















