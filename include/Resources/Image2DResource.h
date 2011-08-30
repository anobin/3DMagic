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

#include "Resource.h"
#include "../Graphics/Image.h"

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
	
	/// standard constructor
	inline Image2DResource(const std::string& name, ResourceManager& manager):
		Resource(name, manager) {}
		
public:
	/// destructor
	virtual ~Image2DResource();
	
	
	/** Get a image represented by this resource
	 * @param image image to place resource data into
	 */
	virtual void getImage(Image* image) const = 0;

};


};



#endif







