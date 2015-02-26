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
/** Header file for Texture class
 *
 * @file Texture.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_TEXTURE_H
#define MAGIC3D_TEXTURE_H

#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif

#include "../Exceptions/MagicException.h"

#include "Image.h"


namespace Magic3D
{

/** Represents a single simple 2D texture 
 */
class Texture
{
public:
	/// the types of minification filters
	enum class MinFilters {
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};
	/// the types of magnification filters
	enum class MagFilters {
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};
	/// the edge wrap modes
	enum class WrapModes {
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, 
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        REPEAT = GL_REPEAT
	};
    // the compare modes
    enum class CompareModes {
        COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE
    };
    // the compare functions
    enum class CompareFuncs {
        LEQUAL = GL_LEQUAL,
        GEQUAL = GL_GEQUAL,
        LESS = GL_LESS,
        GREATER = GL_GREATER,
        EQUAL = GL_EQUAL,
        NOTEQUAL = GL_NOTEQUAL,
        ALWAYS = GL_ALWAYS,
        NEVER = GL_NEVER
    };

	
private:
	/// id of texture on graphics memory
	GLuint tid;

    unsigned int width;
    unsigned int height;
	
public:
	
    Texture(GLenum internalFormat, unsigned int width, unsigned int height);

	/** Standard constructor
	 * @param image the image to build this texture around.
	 * @param generateMipmaps whether to generate mipmaps or not
	 */
	Texture(const Image& image, bool removeGammaCorrection = true, bool generateMipmaps = false);
	
	/// copy constructor
	inline Texture(const Texture& copy)
	{
		throw_MagicException("Texture objects represent segments of graphics "
			"memory and thus should not be copied");
	}
	
	/// destructor
	virtual ~Texture();

    void set(const Image& image, bool removeGammaCorrection = true, bool generateMipmaps = false);
	
	/// bind this texture to be the current texture state
	inline void bind()
	{ glBindTexture(GL_TEXTURE_2D, tid); }
	
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
		glTexParameteri(GL_TEXTURE_2D, parameter, value);
	}
	inline void setParameter(GLenum parameter, GLfloat value) 
	{ 
		this->bind();
		glTexParameterf(GL_TEXTURE_2D, parameter, value);
	}
		
	/** Set the minification filter to use
	 * @param filter the filter to use
	 */
	inline void setMinFilter(MinFilters filter)
	{ this->setParameter(GL_TEXTURE_MIN_FILTER, (int)filter); }

	/** Set the magnification filter to use
	 * @param filter the filter to use
	 */
	inline void setMagFilter(MagFilters filter)
	{ this->setParameter(GL_TEXTURE_MAG_FILTER, (int)filter); }
	
	/** Set the texture wrap mode for both s and t coords
	 * @param mode the wrap mode to set
	 */
	inline void setWrapMode(WrapModes mode)
	{ this->setParameter( GL_TEXTURE_WRAP_S, (int)mode);
	  this->setParameter( GL_TEXTURE_WRAP_T, (int)mode); }

    inline void setCompareMode(CompareModes mode)
    { this->setParameter(GL_TEXTURE_COMPARE_MODE, (int)mode); }

    inline void setCompareFunc(CompareFuncs func)
    { this->setParameter(GL_TEXTURE_COMPARE_FUNC, (int)func); }

    inline unsigned int getWidth() { return this->width; }
    inline unsigned int getHeight() { return this->height; }
};


};








#endif










