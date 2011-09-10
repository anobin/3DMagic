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
/** Contains global Freetype init methods.
 *
 * @file Freetype_init.cpp
 * @author Andrew Keating
 */
 
// include freetype
#include <ft2build.h>
#include FT_FREETYPE_H // yes it's a macro include and yes it's the standard way
 
namespace Magic3D
{
    
int freetype_init_cnt = 0;
FT_Library freetype_library = NULL;
  
/// Init freetype library    
int init_Freetype(FT_Library* library)
{
    int ret;
        
    // init freetype main component if needed
    if (freetype_init_cnt < 1 )
    {
        ret = FT_Init_FreeType(&freetype_library);
        if (ret < 0)
            return ret;
    }
    freetype_init_cnt++;
    (*library) = freetype_library; 
    
    return 0;
}
    
    
/// Quit freetype library
void quit_Freetype()
{           
    // quit freetype if noone needs it anymore
    if (freetype_init_cnt == 0)
        return;
    freetype_init_cnt--;
    if (freetype_init_cnt == 0 )
    {
        FT_Done_FreeType(freetype_library);
        freetype_library = NULL;
    }   
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
};
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
