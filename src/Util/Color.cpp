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
/** Implementation file for Color class 
 *
 * @file Color.cpp
 * @author Andrew Keating
 */

#include <Util/Color.h>

namespace Magic3D
{
	
	
#define RGB_SINGLE(x) ((1.0f/255.0f)*(x))
#define rRGB_SINGLE(x) (int((x)/(1.0f/255.0f)))
	
	
/// default constructor
Color::Color()
{
	// default to pink, to make sure it's noticable a default was ignored
	this->color[0] = RGB_SINGLE(255);
	this->color[1] = RGB_SINGLE(165);
	this->color[2] = RGB_SINGLE(255);
	this->color[3] = RGB_SINGLE(255);
}
	
/** Standard constructor for four-component color values
 * @param c1 the first component
 * @param c2 the second component
 * @param c3 the third component
 * @param c4 the fourth component
 * @param format the format of the color values
 */
Color::Color(float c1, float c2, float c3, float c4, Color::Format format)
{
	switch (format)
	{
		case Color::RGBA_BYTE:
			this->color[0] = RGB_SINGLE(c1);
			this->color[1] = RGB_SINGLE(c2);
			this->color[2] = RGB_SINGLE(c3);
			this->color[3] = RGB_SINGLE(c4);
			break;
			
		case Color::RGBA_FLOAT:
			this->color[0] = c1;
			this->color[1] = c2;
			this->color[2] = c3;
			this->color[3] = c4;
			break;
			
		case Color::RGB_BYTE:
		case Color::RGB_FLOAT:
			throw_MagicException("Tried to create a three-component color using four-component constructor");
		default:
			throw_MagicException("Unknown color format");
	}
}
	
/** Standard constructor for three-component color values
 * @param c1 the first component
 * @param c2 the second component
 * @param c3 the third component
 * @param format the format of the color values
 */
Color::Color(float c1, float c2, float c3, Color::Format format)
{
	switch (format)
	{
		case Color::RGB_BYTE:
			this->color[0] = RGB_SINGLE(c1);
			this->color[1] = RGB_SINGLE(c2);
			this->color[2] = RGB_SINGLE(c3);
			this->color[3] = 1.0f;
			break;
			
		case Color::RGB_FLOAT:
			this->color[0] = c1;
			this->color[1] = c2;
			this->color[2] = c3;
			this->color[3] = 1.0f;
			break;
			
		case Color::RGBA_BYTE:
		case Color::RGBA_FLOAT:
			throw_MagicException("Tried to create a four-component color using three-component constructor");
		default:
			throw_MagicException("Unknown color format");
	}
}

/** Get copy of color data in specified format
 * @param array array to store values in
 * @param format the format of the export data
 */
void Color::getColor(float* array, Color::Format format) const
{
	switch (format)
	{
		case Color::RGB_BYTE:
			array[0] = (float)rRGB_SINGLE(this->color[0]);
			array[1] = (float)rRGB_SINGLE(this->color[1]);
			array[2] = (float)rRGB_SINGLE(this->color[2]);
			break;
			
		case Color::RGB_FLOAT:
			array[0] = this->color[0];
			array[1] = this->color[1];
			array[2] = this->color[2];
			break;
			
		case Color::RGBA_BYTE:
			array[0] = (float)rRGB_SINGLE(this->color[0]);
			array[1] = (float)rRGB_SINGLE(this->color[1]);
			array[2] = (float)rRGB_SINGLE(this->color[2]);
			array[3] = (float)rRGB_SINGLE(this->color[3]);
			break;
			
		case Color::RGBA_FLOAT:
			array[0] = this->color[0];
			array[1] = this->color[1];
			array[2] = this->color[2];
			array[3] = this->color[3];
			break;
			
		default:
			throw_MagicException("Unknown color format");
	}
}
	
	
	
	
// default colors
const Color Color::BLACK(0, 0, 0);
const Color Color::WHITE(255, 255, 255);
const Color Color::GREEN(0, 255, 0);
const Color Color::BLUE(0, 0, 255);
const Color Color::RED(255, 0, 0);
const Color Color::LIGHT_GREEN(192, 255, 192);
const Color Color::ORANGE(255, 128, 0);
const Color Color::YELLOW(255, 255, 0);
const Color Color::PURPLE(192, 0, 192);
const Color Color::GRAY(160, 160, 160);
const Color Color::PINK(255, 192, 192);
const Color Color::LIGHT_BLUE(192, 192, 255);
	
	
	
	
	
};


















