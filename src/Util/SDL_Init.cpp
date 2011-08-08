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
/** Contains global SDL init methods.
 *
 * @file SDL_init.cpp
 * @author Andrew Keating
 */
 
// SDL includes
#include <SDL/SDL.h>
 
namespace Magic3D
{
    
int sdl_init_cnt = 0;
int sdl_init_vid_cnt = 0;
  
/// Init some part of SDL    
int init_SDL(unsigned int flags)
{
    int ret;
        
    // init SDL main component if needed
    if (sdl_init_cnt < 1 )
    {
        ret = SDL_Init(0);
        if (ret < 0)
            return ret;
    }
    sdl_init_cnt++;

    // init video if needed
    if (flags & SDL_INIT_VIDEO && sdl_init_vid_cnt < 1)
    {
        ret = SDL_InitSubSystem( SDL_INIT_VIDEO );
        if (ret < 0)
            return ret;
        sdl_init_vid_cnt++;
    }
    
    return 0;
}
    
    
/// Quit some part of SDL    
void quit_SDL(unsigned int flags)
{       
    // quit video if no one needs it anymore
    if (flags & SDL_INIT_VIDEO && sdl_init_vid_cnt != 0)
    {
        sdl_init_vid_cnt--;
        if ( sdl_init_vid_cnt == 0 )
            SDL_QuitSubSystem( SDL_INIT_VIDEO );
    }
    
    // quit SDL main component if noone needs SDL anymore
    if (sdl_init_cnt == 0)
        return;
    sdl_init_cnt--;
    if (sdl_init_cnt == 0 )
        SDL_Quit();
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
};
 
 
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 
 
