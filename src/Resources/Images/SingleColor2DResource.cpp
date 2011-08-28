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
/** Implementation file for SingleColor2DResource class
 *
 * @file Single2DResource.cpp
 * @author Andrew Keating
 */

#include <Resources/Images/SingleColor2DResource.h>
#include <Exceptions/ResourceNotFoundException.h>
#include <Exceptions/MagicException.h>


namespace Magic3D
{

/** Standard constructor
 * @param color color to make this image
 * @param name name of the resource
 * @param manager the resource manager
 */
SingleColor2DResource::SingleColor2DResource(const Color& color,
	const std::string& name, ResourceManager& manager):	Image2DResource(name, manager)
{
    // set width and height to 1x1
    this->width = 1;
    this->height = 1;
	
	// format is always RGBA
	format = GL_RGBA;
	
	// type is always float, no reason to convert formats from our color format
	type = GL_FLOAT;
   
    // Calculate size of image
    length = sizeof(GLfloat) * 4 * width * height;
    
    // allocate memory for raw image
    imageData = new GLbyte[length];
	
	// set every four bytes to the color selected
	float internal[4];
	color.getColor(internal, 4);
	GLfloat* f = (GLfloat*)imageData;
	for(unsigned int i=0; i < (length/sizeof(GLfloat)); i += 4)
	{
		f[i]   = internal[0]; // red
		f[i+1] = internal[1]; // green
		f[i+2] = internal[2]; // blue
		f[i+3] = internal[3]; // alpha
	}
}
	
	
/// destructor
SingleColor2DResource::~SingleColor2DResource()
{
	delete[] imageData;
}

/** Clone this resource to get a copy allocated on the heap
 * @return copy of this resource allocated on the heap
 * @warning don't use this function unless you need to modify a
 * resource
 */
Resource* SingleColor2DResource::clone() const
{
	return new SingleColor2DResource(*this);
}


/** get the width of the image data
 * @return width of image data
 */
int SingleColor2DResource::getWidth() const
{
	return width;
}
	
/** get the height of the image data
 * @return height of image data
 */
int SingleColor2DResource::getHeight() const
{
	return height;
}
	
/** get the format of the image data
 * @return the format of the image data
 */
GLenum SingleColor2DResource::getFormat() const
{
	return format;
}
	
/** get the type of the image data
 * @return the type of the image data
 */
GLenum SingleColor2DResource::getType() const
{
	return type;
}

/** get the raw image data
 * @param len place to put the length of the image data into, may be NULL
 * @return const pointer to raw image data
 */
const GLbyte* SingleColor2DResource::getImageData(int* len) const
{
	if (len != NULL)
		(*len) = length;
	return imageData;
}
	
/** Get the alignment of the image data (how many bytes are used for each channel)
 * @return the number of bytes used per channel, used to give openGL the GL_UNPACK_ALIGNMENT
 */
int SingleColor2DResource::getDataAlignment() const
{
	// we don't have any padding
	return 1;
}
	
	
	
	
	
	
	
	
};






