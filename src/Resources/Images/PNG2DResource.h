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
/** Header file for PNG2DResource class
 *
 * @file PNG2DResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_PNG_2D_RESOURCE_H
#define MAGIC3D_PNG_2D_RESOURCE_H

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

/** Represents a single 2D PNG image resource
 */
class PNG2DResource : public Image2DResource
{
protected:
	FILE *fp;
	
public:
	/** Standard constructor
	 * @param path full path to PNG image
	 * @param name name of the resource
	 */
	PNG2DResource(const std::string& path, const std::string& name);

	/// destructor
	virtual ~PNG2DResource();

	/** Get a image represented by this resource
	 * @param image image to place resource data into
	 */
	virtual void getImage(Image* image) const;


};


};














#endif





