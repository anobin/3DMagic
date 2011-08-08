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
/** Implementation file for Event class
 *
 * @file Event.cpp
 * @author Andrew Keating
 */

#include <Event/Event.h>


namespace Magic3D
{



bool Event::set(const SDL_Event& event)
{
    switch( event.type)
    {
        case SDL_KEYDOWN:
            data.type = KEY_DOWN;
            data.key.key = (int) event.key.keysym.sym;
            break;
            
        case SDL_KEYUP:
            data.type = KEY_UP;
            data.key.key = (int) event.key.keysym.sym;
            break;
            
        case SDL_MOUSEMOTION:
            data.type = MOUSE_MOTION;
            data.motion.x = event.motion.x;
            data.motion.y = event.motion.y;
            data.motion.xrel = event.motion.xrel;
            data.motion.yrel = event.motion.yrel;
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            data.type  = MOUSE_BUTTON_DOWN;
            data.button.button = (MouseButtons) event.button.button;
            data.button.x = event.button.x;
            data.button.y = event.button.y;
            break;
            
        case SDL_MOUSEBUTTONUP:
            data.type  = MOUSE_BUTTON_UP;
            data.button.button = (MouseButtons) event.button.button;
            data.button.x = event.button.x;
            data.button.y = event.button.y;
            break;
            
        case SDL_VIDEORESIZE:
            data.type = VIDEO_RESIZE;
            data.resize.w = event.resize.w;
            data.resize.h = event.resize.h;
            break;
            
        case SDL_QUIT:
            data.type = QUIT;
            
        default:
            return false;
    }
    return true;
}


















    
};
















