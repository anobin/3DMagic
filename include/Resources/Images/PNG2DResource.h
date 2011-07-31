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
/** Header file for TGA2DResource class
 *
 * @file TGA2DResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_TGA_2D_RESOURCE_H
#define MAGIC3D_TGA_2D_RESOURCE_H

#include "../Image2DResource.h"
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif

namespace Magic3D
{

/** Represents a single 2D TGA image resource
 */
class TGA2DResource : public Image2DResource
{
protected:
	/// default constructor
	//inline TGA2DResource(): Image2DResource() {}
	
	/// width of the image data
	int width;
	
	/// height of the image data
	int height;
	
	/// format of the image data
	GLenum format;
	
	/// type of the image data
	GLenum type;
	
	/// raw image data 
	GLbyte* imageData;
	
	/// length of image data
	int length;
	
public:
	/** Standard constructor
	 * @param path full path to TGA image
	 * @param name name of the resource
	 * @param manager the resource manager
	 */
	TGA2DResource(const char* path, const std::string& name, ResourceManager& manager);
	
	/// copy constructor
	inline TGA2DResource(const TGA2DResource& copy): Image2DResource(copy.name, *copy.manager)
	{
		width = copy.width;
		height = copy.height;
		format = copy.format;
		type = copy.type;
		imageData = (GLbyte*)malloc(copy.length);
		memcpy(imageData, copy.imageData, copy.length);
		length = copy.length;
	}
	
	/// destructor
	virtual ~TGA2DResource();

	/** Clone this resource to get a copy allocated on the heap
	 * @return copy of this resource allocated on the heap
	 * @warning don't use this function unless you need to modify a
	 * resource
	 */
	virtual Resource* clone() const;


	/** get the width of the image data
	 * @return width of image data
	 */
	virtual int getWidth() const;
	
	/** get the height of the image data
	 * @return height of image data
	 */
	virtual int getHeight() const;
	
	/** get the format of the image data
	* @return the format of the image data
	*/
	virtual GLenum getFormat() const;
	
	/** get the type of the image data
	 * @return the type of the image data
	 */
	virtual GLenum getType() const;
	
	/** get the raw image data
	 * @param len place to put the length of the image data into, may be NULL
	 * @return const pointer to raw image data
	 */
	virtual const GLbyte* getImageData(int* len) const;
	
	/** Get the alignment of the image data (how many bytes are used for each channel)
	 * @return the number of bytes used per channel, used o give openGL the GL_UNPACK_ALIGNMENT
	 */
	virtual int getDataAlignment() const;


};


};














#endif





