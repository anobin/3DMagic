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
/** Implementation file for PNG2DResource class
 *
 * @file PNG2DResource.cpp
 * @author Andrew Keating
 */

#include <Resources/Images/PNG2DResource.h>
#include <Exceptions/ResourceNotFoundException.h>
#include <Exceptions/MagicException.h>
#include <Util/magic_assert.h>

// Include libpng header file
#include <png.h>


namespace Magic3D
{
	



/** Standard constructor
 * @param path full path to TGA image
 * @param name name of the resource
 * @param manager the resource manager
 */
PNG2DResource::PNG2DResource(const char* path, const std::string& name, ResourceManager& manager):
	Image2DResource(name, manager)
{
	png_byte signiture [8];
	
    // open the file given, in binary mode
    this->fp = fopen(path, "rb");
    if (!fp)
        throw_MagicException( "Could not open PNG 2D resource file" );
    
    // read in the PNG signiture and check it for validity
    fread(signiture, 1, 8, fp);
    if (png_sig_cmp(signiture, 0, 8))
        throw_MagicException( "Attempted to create PNG resource from a non-PNG file" );

}
	
	
/// destructor
PNG2DResource::~PNG2DResource()
{
	// close file pointer
    fclose(fp);
}

/** Get a image represented by this resource
 * @param image image to place resource data into
 */
void PNG2DResource::getImage(Image* image) const
{
    png_structp png_ptr;
	png_infop info_ptr;
	png_byte color_type;
	png_byte bit_depth;
	int row_width;
    
    // Initalize the read struture 
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    MAGIC_ASSERT ( png_ptr );   // no reason to ever fail by this point

    // Initialize the infor struture
    info_ptr = png_create_info_struct(png_ptr);
    MAGIC_ASSERT ( info_ptr );  // no reason to ever fail by this point

    // Setup the jump point used by png_init_io() when an error occurs during io
    // initalization
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        MAGIC_ASSERT( false );
    }

    // Give libpng the FILE pointer to read data from
    png_init_io(png_ptr, fp);
    
    // tell libpng that we already read the signiture in, thus the file pointer
    // we just gave is 8 bytes ahead
    png_set_sig_bytes(png_ptr, 8);

    // Read the info for the PNG file 
    png_read_info(png_ptr, info_ptr);

    // get the info for the PNG file
    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // determine the number of channels from the PNG color type
    int channels = 0;
    switch( color_type )
    {
        case PNG_COLOR_TYPE_GRAY:
        case PNG_COLOR_TYPE_GRAY_ALPHA:
        case PNG_COLOR_TYPE_PALETTE:
            channels = 1;
            break;
        case PNG_COLOR_TYPE_RGB:
            channels = 3;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            channels = 4;
            break;
        default:
            throw_MagicException( "Unknown or Unsupported format/color type for PNG 2D resource" );
    }
    
    // determine the openGL type from the PNG bit depth
    switch ( bit_depth )
    {
        case 8:
            break; //perfect, can copy byte per byte into native image format
        case 16:    // TODO: add support for other bit depths
        case 1:
        case 2:
        case 4:
        default:
            throw_MagicException( "Unknown or Unsupported type/bit depth for PNG 2D resource" );
    }
    
    // allocate the output image
    image->allocate(width, height, channels);

    // allocate temporary buffer to place PNG data into
    row_width = png_get_rowbytes(png_ptr,info_ptr);
    MAGIC_ASSERT( row_width >= (width*channels) ); // sanity check
    int length = row_width * height;
    unsigned char* imageData = new unsigned char[ length ];
    
    // setup pointers to rows for reading in using libpng
    // note that PNG stores it's rows in reverse order
    png_bytep* row_pointers =  new png_bytep[ height ];
    for (int i=0, j = height-1; i<height; i++, j--)
        row_pointers[i] = (png_bytep) &imageData[ row_width * j ];
        
    // Setup the jump point used by png_read_image() if an error occurs
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        delete[] row_pointers;
        delete[] imageData;
        throw_MagicException( "Error reading in PNG 2D resource file data" );
    }

    // read in PNG data
    png_read_image(png_ptr, row_pointers);
    
    // free png read and info structures
    png_read_end( png_ptr, NULL );
    png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
    
    // copy temporary buffer into image buffer, this step is needed becuase
    // there may be row padding in the temporary buffer
    unsigned char* data = image->getMutableRawData();
    for(int i=0; i < height; i++)
        memcpy(&data[i*width*channels], &imageData[i*row_width], width*channels);
            
    // free temporary buffer data
    delete[] row_pointers;
    delete[] imageData;
}
	
	
	
	
	
	
	
	
};






