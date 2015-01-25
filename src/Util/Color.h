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
/** Header file for Color class
 *
 * @file Color.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_COLOR_H
#define MAGIC3D_COLOR_H

#include <Exceptions/MagicException.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "magic_throw.h"
#include <Resources\Resource.h>

// everything is in 3DMagic namespace
namespace Magic3D
{
    
#define RGB_byte2float(x) ((1.0f/255.0f)*(float(x)))
#define RGB_float2byte(x) ((unsigned char)((x)/(1.0f/255.0f)))
	
/** Represents a single color. Can be a variable number of channels (though the
 * channels always follow the RGBA format), but the type of each channel is
 * a single byte. There is no channel or color padding. Input can be float
 * format (0.0 to 1.0) or byte format (same as internal).
 */
class Color : public Resource
{
private:
	/// Internal color storage, RGBA, single byte per channel
	unsigned char color[4];
	
	/// number of channels in this color
	int channels;
	
public:	
	Color(int channels): channels(channels) 
	{
	    MAGIC_THROW(channels < 1 || channels > 4, "Channel count out of range." );
	}
	
	/** Standard constructor for four-component float colors
	 * @param c1 the first component
	 * @param c2 the second component
	 * @param c3 the third component
	 * @param c4 the fourth component
	 */
	 inline Color(float c1, float c2, float c3, float c4): channels(4)
	{
	    this->color[0] = RGB_float2byte(c1);
	    this->color[1] = RGB_float2byte(c2);
        this->color[2] = RGB_float2byte(c3);
        this->color[3] = RGB_float2byte(c4);
	}
	
	/** Standard constructor for four-component byte colors
	 * @param c1 the first component
	 * @param c2 the second component
	 * @param c3 the third component
	 * @param c4 the fourth component
	 */
	 inline Color(int c1, int c2, int c3, int c4): channels(4)
	 {
	     this->color[0] = c1;
	     this->color[1] = c2;
	     this->color[2] = c3;
	     this->color[3] = c4;
	 }
	
	
	/** Standard constructor for three-component float colors
	 * @param c1 the first component
	 * @param c2 the second component
	 * @param c3 the third component
	 */
	inline Color(float c1, float c2, float c3): channels(3)
	{
	    this->color[0] = RGB_float2byte(c1);
	    this->color[1] = RGB_float2byte(c2);
	    this->color[2] = RGB_float2byte(c3);
	}
	
	/** Standard constructor for three-component byte colors
	 * @param c1 the first component
	 * @param c2 the second component
	 * @param c3 the third component
	 */
	inline Color(int c1, int c2, int c3): channels(3)
	{
	    this->color[0] = c1;
	    this->color[1] = c2;
	    this->color[2] = c3;
	}
	
	inline Color(float c): channels(1)
	{
	    this->color[0] = RGB_float2byte(c);
	}
	
	inline Color(unsigned char c): channels(1)
	{
	    this->color[0] = c;
	}
	
	inline Color(float* f, int channels): channels(channels)
	{
	    MAGIC_THROW(channels < 1 || channels > 4, "Channel count out of range." );
	    for(int i=0; i < channels; i++)
	        color[i] = RGB_float2byte(f[i]);
	}
	
	inline Color(int* b, int channels): channels(channels)
	{
	    MAGIC_THROW(channels < 1 || channels > 4, "Channel count out of range." );
	    for(int i=0; i < channels; i++)
	        color[i] = b[i];
	}
	
	/** Copy constructor
	 * @param copy the color object to copy
	 */
	inline Color(const Color& copy)
	{
		this->set(copy);
	}
	
	inline int getChannelCount()
	{
	    return this->channels;
	}
	
	/** Set this color to another color
	 * @param copy the color to set to
	 */
	inline void set(const Color& copy)
	{
		this->color[0] = copy.color[0];
		this->color[1] = copy.color[1];
		this->color[2] = copy.color[2];
		this->color[3] = copy.color[3];
		this->channels = copy.channels;
	}
	
	inline float getChannel(int channel, bool float_type) const
	{
	    // special case for requesting alpha channel out of a 3 channel color
	    if (channel == 3 && this->channels == 3)
	        return 1.0f; // assume alpha is 1.0f
	    MAGIC_THROW(channel < 0 || channel >= this->channels, "Attempted to get a non-existent color channel");
	    return RGB_byte2float(this->color[channel]);
	}
	
	inline int getChannel(int channel) const
	{
	    // special case for requesting alpha channel out of a 3 channel color
	    if (channel == 3 && this->channels == 3)
	        return 255; // assume alpha is 255
	    MAGIC_THROW(channel < 0 || channel >= this->channels, "Attempted to get a non-existent color channel");
	    return this->color[channel];
	}
	
	inline void setChannel(float f, int channel)
	{
	    MAGIC_THROW(channel < 0 || channel >= this->channels, "Attempted to set a non-existent color channel");
	    this->color[channel] = RGB_float2byte(f);
	}
	
	inline void setChannel(int b, int channel)
	{
	    MAGIC_THROW(channel < 0 || channel >= this->channels, "Attempted to set a non-existent color channel");
	    this->color[channel] = b;
	}
	
	inline void changeChannelCount(int channels)
	{
	    MAGIC_THROW(channels < 1 || channels > 4, "Channel count out of range." );
	    this->channels = channels;
	}
	
	/** Get copy of color data in float format
	 * @param array array to store values in
	 */
	inline void getColor(float* array, int channels) const
	{
	    // special case for requesting 4 channels out of a 3 channel color
	    if (channels == 4 && this->channels == 3)
	        array[3] = 1.0f; // assume alpha is 1
	    else
	        MAGIC_THROW(channels > this->channels, "Tried to get too many channels." );
	    for( int i = 0; i < this->channels; i++)
	        array[i] = RGB_byte2float(this->color[i]);
	}
	
	/** Get copy of color data in byte format
	 * @param array array to store values in
	 */
	inline void getColor(unsigned char* array, int channels) const
	{
	    // special case for requesting 4 channels out of a 3 channel color
	    if (channels == 4 && this->channels == 3)
	        array[3] = 255; // assume alpha is 255
	    else
	        MAGIC_THROW(channels > this->channels, "Tried to get too many channels." );
	    for( int i = 0; i < this->channels; i++)
	        array[i] = color[i];
	}
	
	inline void setColor(float* array)
	{
	    for(int i=0; i < this->channels; i++)
	        this->color[i] = RGB_float2byte(array[i]);
	}
	
	inline void setColor(unsigned char* array)
	{
	    for(int i=0; i < this->channels; i++)
	        this->color[i] = array[i];
	}
	
	inline int get() const { return this->getRed(); }
	inline int getRed() const { return this->getChannel(1); }
	inline int getGreen() const { return this->getChannel(2); }
	inline int getBlue() const { return this->getChannel(3); }
	inline int getAlpha() const { return this->getChannel(4); }
	
	inline void set(int b) { this->setRed(b); }
	inline void setRed(int b) { this->setChannel(b, 1); }
	inline void setGreen(int b) { this->setChannel(b, 2); }
	inline void setBlue(int b) { this->setChannel(b, 3); }
	inline void setAlpha(int b) { this->setChannel(b, 4); }
	
	
	// default colors
	static const Color BLACK;
	static const Color WHITE;
	static const Color GREEN;
	static const Color BLUE;
	static const Color RED;
	static const Color LIGHT_GREEN;
	static const Color ORANGE;
	static const Color YELLOW;
	static const Color PURPLE;
	static const Color GRAY;
	static const Color PINK;
	static const Color LIGHT_BLUE;
	
	
};
	
	
	
	
	
	
	
	
	
}


#endif











