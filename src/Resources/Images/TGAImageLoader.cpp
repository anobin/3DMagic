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

#include "TGAImageLoader.h"
#include <Exceptions/ResourceNotFoundException.h>
#include <Exceptions/MagicException.h>
#include <Util/magic_assert.h>

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


std::shared_ptr<Image> TGAImageLoader::getImage(const std::string& path) const
{
	// Attempt to open the file
    FILE* pFile = fopen(path.c_str(), "rb");
    if(pFile == NULL)
        throw_ResourceNotFoundException("Could not find TGA image file");

    TGAHEADER tgaHeader;		// TGA file header
    
    // read in header
    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
	
    // extract width, height, and channel count
    int width = tgaHeader.width;
    int height = tgaHeader.height;
    int channels = tgaHeader.bits / 8; // number of channels / bytes per pixel
    
    // we can only handle 8, 24, or 32 bits per pixel
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        throw_MagicException("Can not handle TGA image with other than 8,24, or 32 bits per pixel");
	
    // Calculate size of image buffer
    int length = width * height * channels;
    unsigned char* tempData = new unsigned char[length];
    
    // allocate memory in image
	std::shared_ptr<Image> image = std::make_shared<Image>();
    image->allocate(width, height, channels );
    
    // read in raw image data
    if(fread(tempData, length, 1, pFile) != 1)
		throw_MagicException("Could not read TGA image data");
	
	// change format of pixels in temporary buffer
	// TGA format pixels are BGRA instead of RGBA
	if (channels != 1) // no reformat at all for single channel
	{
        unsigned char t;
        for(int i=0; i < width*height*channels; i += channels)
        {
            t = tempData[i];
            tempData[i]   = tempData[i+2]; // RED
            // GREEN is in right place
            tempData[i+2] = t;   // BLUE
            // ALPHA is in right place
        }
    }
	
	// copy temp raw data into image raw data
	memcpy(image->getMutableRawData(), tempData, width*height*channels);
	
	// delete temporary image data
	delete[] tempData;

	// close image file
	fclose(pFile);

	return image;
}
	
	
	
	
	
	
	
	
};






