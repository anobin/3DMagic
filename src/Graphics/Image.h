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
/** Header file for Image class 
 *
 * @file Image.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_IMAGE_H
#define MAGIC3D_IMAGE_H

#include "../Util/Color.h"
#include "../Util/magic_assert.h"
#include <string.h>

namespace Magic3D
{
    
class StaticFont;
    

/** Represent image data on system memory. Can come from image resources or be
 * created manually. Can be moved to video memory by creating a Texture based
 * on this image or copying this image to an existing texture.
 *
 * The data in this class can contain a number of channels (though channel layout
 * is always RGBA for more than one channel), but the data type for every channel
 * is always a single byte with no padding.
 */
class Image
{
protected:
    /// width of image data
    int width;
    
    /// height of image data
    int height;
    
    /// number of channels in image (single up to RGBA)
    int channels;
    
    /// raw data for image, no channel, pixel, or row padding
    unsigned char* data;

public:
    /// constructor for undefined image, must be allocate()'d later
    inline Image(): width(0), height(0), channels(0), data(NULL) {}
    
    /// destructor
    virtual ~Image();

    /// constructor to allocate, but not specify, image data
    inline Image(int width, int height, int channels): width(width), height(height),
        channels(channels)
    {
        MAGIC_THROW( channels < 1 || channels > 4, "Channel count out of range." );
        this->data = new unsigned char[width*height*channels];
    }
    
    /// constructor for single color image
    inline Image(int width, int height, int channels, const Color& c): 
        width(width), height(height), channels(channels)
    {
        MAGIC_THROW( channels < 1 || channels > 4, "Channel count out of range." );
        this->data = new unsigned char[width*height*channels];
        unsigned char p[4];
        c.getColor(p, channels);
        for(int i = 0; i < width*height*channels; i += channels)
            memcpy(&data[i], p, channels);
    }
    
    /// copy constructor
    inline Image( const Image& copy): width(copy.width), height(copy.height), 
        channels(copy.channels)
    {
        this->data = new unsigned char[width*height*channels];
        memcpy(this->data, copy.data, width*height*channels );
    }
    
    inline void set(const Image& copy)
    {
        this->width = copy.width;
        this->height = copy.height;
        this->channels = copy.channels;
        delete this->data;
        this->data = new unsigned char[width*height*channels];
        memcpy(this->data, copy.data, width*height*channels );
    }
    
    inline void allocate(int width, int height, int channels)
    {
        if (this->width != width || this->height != height || this->channels != channels)
        {
            this->width = width;
            this->height = height;
            this->channels = channels;
            delete this->data;
            this->data = new unsigned char[width*height*channels];
        }
    }
    
    inline int getWidth() const
    {
        return this->width;
    }
    
    inline int getHeight() const
    {
        return this->height;
    }
    
    inline int getChannelCount() const
    {
        return this->channels;
    }
    
    // don't mess with the raw data unless you are doing major operations
    inline unsigned char* getMutableRawData(int* length = NULL)
    {
        if (length)
            (*length) = width*height*channels;
        return this->data;
    }
    
    /** Get the raw data for this image in a read-only fashion
     * @param length optional length parameter to retrieve length of data,
     * the length of the data is always width*height*channels.
     * @return pointer to raw data
     */
    inline const unsigned char* getRawData(int* length = NULL) const
    {
        if (length)
            (*length) = width*height*channels;
        return this->data;
    }

    inline void getPixel(Color* p, int x, int y) const 
    {
        MAGIC_THROW( x >= width, "X component out of range" );
        MAGIC_THROW( y >= height, "Y component out of range" );
        p->changeChannelCount(this->channels);
        p->setColor(&this->data[x*y*channels]);
    }
    
    inline void setPixel(const Color& p, int x, int y)
    {
        MAGIC_THROW( x >= width, "X component out of range" );
        MAGIC_THROW( y >= height, "Y component out of range" );
        unsigned char c[4];
        p.getColor(c, this->channels);
        memcpy(&this->data[x*y*channels], c, this->channels);
    }
    
    inline void rectFill(const Color& p, int x, int y, int width, int height)
    {
        MAGIC_THROW( (x+width) > this->width, "Width of rect too large.");
        MAGIC_THROW( (y+height) > this->height, "Height of rect too large.");
        
        unsigned char c[4];
        p.getColor(c, channels);
        int end = (x+width) * (y+height) * channels;
        for(int i = x*y*channels; i < end; i += channels)
            memcpy(&data[i], c, channels);
    }
    
    inline void clear(const Color& p)
    {
        this->rectFill(p, 0,0, this->width, this->height);
    }

    inline void copyIn(const Image& source, int destX = 0, int destY = 0,
        int sourceX = 0, int sourceY = 0, int width = -1, 
        int height = -1 )
    {
        Image::copyImage(this, source, destX, destY, sourceX, sourceY, width, height);
    }
    
    inline void copyOut(Image* dest, int destX = 0, int destY = 0,
        int sourceX = 0, int sourceY = 0, int width = -1, 
        int height = -1 )
    {
        Image::copyImage(dest, *this, destX, destY, sourceX, sourceY, width, height);
    }

    static void copyImage(Image* dest, const Image& source, int destX = 0, 
        int destY = 0, int sourceX = 0, int sourceY = 0, int width = -1, 
        int height = -1 );
    
    inline void blend(const Image& source, int destX = 0, int destY = 0, 
        int sourceX = 0, int sourceY = 0, int width = -1, int height = -1 )
    {
        Image::blendImage(this, source, destX, destY, sourceX, sourceY, width, height );
    }

    static void blendImage(Image* dest, const Image& source, int destX = 0,
        int destY = 0, int sourceX = 0, int sourceY = 0, int width = -1,
        int height = -1 );
    
    void drawAsciiText(const StaticFont& font, const char* str, int x, int y, const Color& color);
    
};


};








#endif


