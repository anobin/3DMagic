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

#include <RectTexture.h>

namespace Magic3D
{
	
/** Standard constructor
 * @param image the image to build the texture around
 */
RectTexture::RectTexture(const Image2DResource* image)
{
	// generate texture id
	glGenTextures(1, &tid);
	
	// bind to our state
	glBindTexture(GL_TEXTURE_RECTANGLE, tid);
	
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
	glTexImage2D(GL_TEXTURE_RECTANGLE,		// 2D rectangular image data 
				 0, 						// base mipmap level, only level allowed
				 internalFormat,			// the graphics memory format we want it in
				 image->getWidth(),			// width of image
				 image->getHeight(),		// height of image
				 0,							// this parameter is always 0. :?
				 image->getFormat(),		// format of image (layout of channels) 
				 image->getType(), 			// image data type (size per channel)
				 image->getImageData(NULL));// actual data
				 
	// set min filter to something other than mipmap'ed, cuase it ain't allowed
	this->setMinFilter(RectTexture::MIN_LINEAR);
}

/// destructor
RectTexture::~RectTexture()
{
	// delete texture state and graphics memory
	glDeleteTextures(1, &tid);
}
	
	

	
	
};


























