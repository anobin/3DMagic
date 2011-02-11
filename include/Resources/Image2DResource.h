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
/** Header file for Image2DResource class
 *
 * @file Image2DResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_IMAGE_2D_RESOURCE_H
#define MAGIC3D_IMAGE_2D_RESOURCE_H

#include "../Resource.h"

#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif

namespace Magic3D
{

/** Base class for all 2D image resources
 */
class Image2DResource : public Resource
{
protected:
	/// default constructor
	inline Image2DResource(): Resource() {}
	
	/// standard constructor
	inline Image2DResource(const std::string& name, ResourceManager& manager):
		Resource(name, manager) {}
		
public:
	/// destructor
	virtual ~Image2DResource();
	
	/** Clone this resource to get a copy allocated on the heap
	 * @return copy of this resource allocated on the heap
	 * @warning don't use this function unless you need to modify a
	 * resource
	 */
	virtual Resource* clone() const = 0;

	/** get the width of the image data
	 * @return width of image data
	 */
	virtual int getWidth() const = 0;
	
	/** get the height of the image data
	 * @return height of image data
	 */
	virtual int getHeight() const = 0;
	
	/** get the format of the image data
	* @return the format of the image data
	*/
	virtual GLenum getFormat() const = 0;
	
	/** get the type of the image data
	 * @return the type of the image data
	 */
	virtual GLenum getType() const = 0;
	
	/** get the raw image data
	 * @param len place to put the length of the image data into, may be NULL
	 * @return const pointer to raw image data
	 */
	virtual const GLbyte* getImageData(int* len) const = 0;
	
	/** Get the alignment of the image data (how many bytes are used for each channel)
	 * @return the number of bytes used per channel, used o give openGL the GL_UNPACK_ALIGNMENT
	 */
	virtual int getDataAlignment() const = 0;

};


};



#endif







