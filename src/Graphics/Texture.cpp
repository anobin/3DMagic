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
/** Implementation file for Texture class
 * 
 * @file Texture.cpp
 * @author Andrew Keating
 */

#include <Graphics/Texture.h>
#include <Util/magic_assert.h>

namespace Magic3D
{
	
Texture::Texture(GLenum internalFormat, unsigned int width, unsigned int height) :
    width(width), height(height)
{
    glGenTextures(1, &tid);

    this->bind();
    glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
}

/** Standard constructor
 * @param image the image to build this texture around.
 * @param generateMipmaps whether to generate mipmaps or not
 */
Texture::Texture(const Image& image, bool removeGammaCorrection, bool generateMipmaps) :
    width(image.getWidth()), height(image.getHeight())
{
    // generate texture id
	glGenTextures(1, &tid);
	
    this->set(image, removeGammaCorrection, generateMipmaps);
}

void Texture::set(const Image& image, bool removeGammaCorrection, bool generateMipmaps)
{
    // bind to our state
	glBindTexture(GL_TEXTURE_2D, tid);
	
	// no row alignment in Image class
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	// figure out the internal format we want to use, we perfer compression
	GLint internalFormat = 0;
	GLint format = 0;
	switch(image.getChannelCount())
	{
	    case 1:
	        format = GL_RED;
			internalFormat = GL_RED;
			break;
			
		case 2:
		    format = GL_RG;
			internalFormat = GL_RG;
			break;
			
		case 3:
		    format = GL_RGB;
            if (removeGammaCorrection)
			    internalFormat = GL_SRGB;
            else
                internalFormat = GL_RGB;
			break;
			
		case 4:
		    format = GL_RGBA;
            if (removeGammaCorrection)
			    internalFormat = GL_SRGB_ALPHA;
            else
                internalFormat = GL_RGBA;
			break;
			
		default:
		    MAGIC_ASSERT(false);
	}
	
	// unpack data into graphics memory
	glTexImage2D(GL_TEXTURE_2D,			// 2D image data 
				 0, 					// base mipmap level
				 internalFormat,	    // the graphics memory format we want it in
				 image.getWidth(),			// width of image
				 image.getHeight(),		    // height of image
				 0,					    // this parameter is always 0. :?
				 format,		        // format of image (layout of channels) 
				 GL_UNSIGNED_BYTE,      // image data type (size per channel)
				 image.getRawData());           // actual data
				 
	// generate mipmaps if instructed to
	if (generateMipmaps)
		 glGenerateMipmap(GL_TEXTURE_2D);
	else
	{
		// if there is no mipmap, then set the min filter to something that
		// will work without a mipmap
		this->setMinFilter(Texture::MinFilters::LINEAR);
	}
}
	
/// destructor
Texture::~Texture()
{
	// delete texture state and graphics memory
	glDeleteTextures(1, &tid);
}
	
	
		
	
	
	
	
	
	
	
	
	
	
	
};