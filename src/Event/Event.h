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
/** Header file for Event class
 *
 * @file Event.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_EVENT_H
#define MAGIC3D_EVENT_H


// SDL includes
#include <SDL/SDL.h>


namespace Magic3D
{

/** Represents a single event in the system
 */
class Event
{
public:
    /// types of events
    enum Types
    {
        KEY_DOWN            = SDL_KEYDOWN,
        KEY_UP              = SDL_KEYUP,
        MOUSE_MOTION        = SDL_MOUSEMOTION,
        MOUSE_BUTTON_DOWN   = SDL_MOUSEBUTTONDOWN,
        MOUSE_BUTTON_UP     = SDL_MOUSEBUTTONUP,
        VIDEO_RESIZE        = SDL_VIDEORESIZE,
        QUIT                = SDL_QUIT
    };
    
    /// mouse buttons
    enum MouseButtons
    {
        LEFT        = SDL_BUTTON_LEFT, 
        MIDDLE      = SDL_BUTTON_MIDDLE, 
        RIGHT       = SDL_BUTTON_RIGHT, 
        WHEEL_UP    = SDL_BUTTON_WHEELUP, 
        WHEEL_DOWN  = SDL_BUTTON_WHEELDOWN
    };

    /// keyboard event type
    struct KeyEvent
    {
        Types type;
        int key;
    };
    
    /// mouse motion event type
    struct MouseMotionEvent
    {
        Types type;
        int x, y;
        int xrel, yrel;
    };
    
    /// mouse button event type
    struct MouseButtonEvent
    {
        Types type;
        MouseButtons button;
        int x, y;
    };
    
    /// video resize event
    struct VideoResizeEvent
    {
        Types type;
        int w, h;
    };
    
    
    /// event data type
    union Data
    {
        Types type;
        KeyEvent key;
        MouseMotionEvent motion;
        MouseButtonEvent button;
        VideoResizeEvent resize;
    };


    Data data;


    inline Event()
    {
    }
        
    bool set(const SDL_Event& event);


};


};


#endif










