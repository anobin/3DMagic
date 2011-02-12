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

namespace Magic3D
{
	
/** Standard constructor
 * @param image the image to build this texture around. We don't
 * accept (and hold) a resource handle becuase we only care about memory
 * on the graphics processor, not the main memory. If textures are loaded on
 * and off graphics memory, then hold onto the handles and create a new texture
 * when you want it back on graphics memory.
 * @param generateMipmaps whether to generate mipmaps or not
 */
Texture::Texture(const Image2DResource* image, bool generateMipmaps)
{
	// generate texture id
	glGenTextures(1, &tid);
	
	// bind to our state
	glBindTexture(GL_TEXTURE_2D, tid);
	
	// set data alignment before unpacking
	glPixelStorei(GL_UNPACK_ALIGNMENT, image->getDataAlignment());
	
	// figure out the internal format we want to use, we perfer compression
	GLint internalFormat = 0;
	switch(image->getFormat())
	{
		case GL_RED:
			internalFormat = GL_COMPRESSED_RED;
			break;
			
		case GL_RG:
			internalFormat = GL_COMPRESSED_RG;
			break;
			
		case GL_RGB:
		case GL_BGR:
			internalFormat = GL_COMPRESSED_RGB;
			break;
			
		case GL_RGBA:
		case GL_BGRA:
		default:
			internalFormat = GL_COMPRESSED_RGBA;
	}
	
	// unpack data into graphics memory
	glTexImage2D(GL_TEXTURE_2D,				// 2D image data 
				 0, 						// base mipmap level
				 internalFormat,			// the graphics memory format we want it in
				 image->getWidth(),			// width of image
				 image->getHeight(),		// height of image
				 0,							// this parameter is always 0. :?
				 image->getFormat(),		// format of image (layout of channels) 
				 image->getType(), 			// image data type (size per channel)
				 image->getImageData(NULL));// actual data
				 
	// generate mipmaps if instructed to
	if (generateMipmaps)
		 glGenerateMipmap(GL_TEXTURE_2D);
	else
	{
		// if there is no mipmap, then set the min filter to something that
		// will work without a mipmap
		this->setMinFilter(Texture::MIN_LINEAR);
	}
	
}
	
/// destructor
Texture::~Texture()
{
	// delete texture state and graphics memory
	glDeleteTextures(1, &tid);
}
	
	
		
	
	
	
	
	
	
	
	
	
	
	
};