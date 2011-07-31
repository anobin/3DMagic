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
/** Header file for RectTexture class
 *
 * @file RectTexture.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_RECT_TEXTURE_H
#define MAGIC3D_RECT_TEXTURE_H

#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif

#include "../Exceptions/MagicException.h"

#include "../Resources/Image2DResource.h"


namespace Magic3D
{

/** Represents a 'Rectangular' texture that is not normalized
 * by openGL; which means that the texture coords map directly
 * to pixel coordinates when used with a orthographic projection. 
 * This class is useful for GUI's and HUD's
 */
class RectTexture
{
public:
	/// the types of minification filters, no mipmaps for rectangular
	enum MinFilters {
		MIN_NEAREST = GL_NEAREST,
		MIN_LINEAR = GL_LINEAR
	};
	/// the types of magnification filters
	enum MagFilters {
		MAG_NEAREST = GL_NEAREST,
		MAG_LINEAR = GL_LINEAR
	};
	/// the edge wrap modes, no repeating modes for rectangular
	enum WrapModes {
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, 
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
	};

private:
	/// id of texure on graphics memory 
	GLuint tid;

	/// default constructor
	inline RectTexture(): tid(0) {}

public:
	/** Standard constructor
	 * @param image the image to build the texture around
	 */
	RectTexture(const Image2DResource* image);

	/// copy constructor
	inline RectTexture(const RectTexture& copy)
	{
		throw_MagicException("Texture objects represent segments of graphics "
			"memory and thus should not be copied");
	}

	/// destructor
	virtual ~RectTexture();

	/// bind this texture to be the current texture state
	inline void bind()
	{ glBindTexture(GL_TEXTURE_RECTANGLE, tid); }
	
	/// get texture id
	inline GLuint getID() const
	{ return this->tid; }

	/** Set a texture parameter, basically a link to glTexParameter
	 * @param parameter the parameter to set
	 * @param value the value to set the parameter to
	 * @warning only call this directly if no other method suits your needs,
	 * as this does no checks to ensure the correct parameters are set correctly
	 */
	inline void setParameter(GLenum parameter, GLint value) 
	{ 
		this->bind();
		glTexParameteri(GL_TEXTURE_RECTANGLE, parameter, value);
	}
	inline void setParameter(GLenum parameter, GLfloat value) 
	{ 
		this->bind();
		glTexParameterf(GL_TEXTURE_RECTANGLE, parameter, value);
	}
		
	/** Set the minification filter to use
	 * @param filter the filter to use
	 */
	inline void setMinFilter(MinFilters filter)
	{ this->setParameter(GL_TEXTURE_MIN_FILTER, filter); }

	/** Set the magnification filter to use
	 * @param filter the filter to use
	 */
	inline void setMagFilter(MagFilters filter)
	{ this->setParameter(GL_TEXTURE_MAG_FILTER, filter); }
	
	/** Set the texture wrap mode for both s and t coords
	 * @param mode the wrap mode to set
	 */
	inline void setWrapMode(WrapModes mode)
	{ this->setParameter( GL_TEXTURE_WRAP_S, mode);
	  this->setParameter( GL_TEXTURE_WRAP_T, mode); }

};

};









#endif














