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
/** Implementation file for TGA2DResource class
 *
 * @file TGA2DResource.cpp
 * @author Andrew Keating
 */

#include <Resources/Images/TGA2DResource.h>
#include <Exceptions/ResourceNotFoundException.h>
#include <Exceptions/MagicException.h>

#ifdef _WIN32
#pragma pack(push, 1)
#define PACKED
#else
#define PACKED __attribute__ ((packed))
#endif


namespace Magic3D
{
	
// header format of TGA image
typedef struct
{
    GLbyte	identsize;              // Size of ID field that follows header (0)
    GLbyte	colorMapType;           // 0 = None, 1 = paletted
    GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart PACKED;  // First colour map entry
    unsigned short	colorMapLength PACKED; // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart PACKED;         // image x origin
    unsigned short	ystart PACKED;         // image y origin
    unsigned short	width PACKED;          // width in pixels
    unsigned short	height PACKED;         // height in pixels
    GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte	descriptor;             // image descriptor
} TGAHEADER;


#ifdef WIN32
#pragma pack(pop)
#endif
#undef PACKED


/** Standard constructor
 * @param path full path to TGA image
 * @param name name of the resource
 * @param manager the resource manager
 */
TGA2DResource::TGA2DResource(const char* path, const std::string& name, ResourceManager& manager):
	Image2DResource(name, manager)
{
	// needed vars
	FILE *pFile;			// File pointer
    TGAHEADER tgaHeader;		// TGA file header
    short imageDepth;				// Pixel depth;
    
    // Attempt to open the file
    pFile = fopen(path, "rb");
    if(pFile == NULL)
        throw ResourceNotFoundExceptionMacro("Could not find TGA image file");
	
    // read in header
    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
	
    // extract width, height, and depth of image
    width = tgaHeader.width;
    height = tgaHeader.height;
    imageDepth = tgaHeader.bits / 8;
    
    // we can only handle 8, 24, or 32 bits per pixel
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        throw MagicExceptionMacro("Can not handle TGA image with other than 8,24, or 32 bits per pixel");
	
    // Calculate size of image buffer
    length = tgaHeader.width * tgaHeader.height * imageDepth;
    
    // allocate memory for raw image
    imageData = new GLbyte[length];
    
    // read in raw image data
    if(fread(imageData, length, 1, pFile) != 1)
	{
        delete[] imageData;
		throw MagicExceptionMacro("Could not read TGA image data");
	}
    
    // Set OpenGL format expected and figure out length
    switch(imageDepth)
	{
        case 3:     // Most likely case
            format = GL_BGR;
            break;
        case 4:
            format = GL_BGRA;
            break;
        case 1:
            format = GL_LUMINANCE;
            break;
	}
	
	// close image file
	fclose(pFile);
	
	// type is always unsigned byte for TGA
	type = GL_UNSIGNED_BYTE;
}
	
	
/// destructor
TGA2DResource::~TGA2DResource()
{
	delete[] imageData;
}

/** Clone this resource to get a copy allocated on the heap
 * @return copy of this resource allocated on the heap
 * @warning don't use this function unless you need to modify a
 * resource
 */
Resource* TGA2DResource::clone() const
{
	return new TGA2DResource(*this);
}


/** get the width of the image data
 * @return width of image data
 */
int TGA2DResource::getWidth() const
{
	return width;
}
	
/** get the height of the image data
 * @return height of image data
 */
int TGA2DResource::getHeight() const
{
	return height;
}
	
/** get the format of the image data
 * @return the format of the image data
 */
GLenum TGA2DResource::getFormat() const
{
	return format;
}
	
/** get the type of the image data
 * @return the type of the image data
 */
GLenum TGA2DResource::getType() const
{
	return type;
}

/** get the raw image data
 * @param len place to put the length of the image data into, may be NULL
 * @return const pointer to raw image data
 */
const GLbyte* TGA2DResource::getImageData(int* len) const
{
	if (len != NULL)
		(*len) = length;
	return imageData;
}
	
/** Get the alignment of the image data (how many bytes are used for each channel)
 * @return the number of bytes used per channel, used o give openGL the GL_UNPACK_ALIGNMENT
 */
int TGA2DResource::getDataAlignment() const
{
	// TGA alignment never has padding
	return 1;
}
	
	
	
	
	
	
	
	
};






