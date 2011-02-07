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

#include <MagicException.h>
#include <GL/glew.h>
#include <GL/gl.h>

// everything is in 3DMagic namespace
namespace Magic3D
{
	
/** Represents a single color, usable in
 * multiple color systems (RGBAf, RGBAb)
 */
class Color
{
private:
	/// Internal color storage, RGBAf (openGL default)
	GLfloat color[4]; // solid black
	
public:
	/// types of color formats
	enum Format { RGBAb, RGBAf, RGBb, RGBf };
	
	/// default constructor
	Color();
	
	/** Standard constructor for four-component color values
	 * @param c1 the first component
	 * @param c2 the second component
	 * @param c3 the third component
	 * @param c4 the fourth component
	 * @param format the format of the color values
	 */
	Color(float c1, float c2, float c3, float c4, Format format = RGBAb);
	
	/** Standard constructor for three-component color values
	 * @param c1 the first component
	 * @param c2 the second component
	 * @param c3 the third component
	 * @param format the format of the color values
	 */
	Color(float c1, float c2, float c3, Format format = RGBb);
	
	/** Copy constructor
	 * @param copy the color object to copy
	 */
	inline Color(const Color& copy)
	{
		this->set(copy);
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
	}
	
	/** Get copy of color data in specified format
	 * @param array array to store values in
	 * @param format the format of the export data
	 */
	void getColor(float* array, Format format = RGBAb) const;
	
	/** Get read-only access to internal color data (RGBAf)
	 * @return the internal color data
	 */
	inline const GLfloat* getInternal() const
	{
		return this->color;
	}
	
	
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











