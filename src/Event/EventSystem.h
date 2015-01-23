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
/** Header file for EventSystem class
 *
 * @file EventSystem.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_EVENT_SYSTEM_H
#define MAGIC3D_EVENT_SYSTEM_H

// SDL includes
#include <SDL/SDL.h>


#include "../Exceptions/MagicException.h"
#include "../Util/Color.h"
#include "Event.h"

namespace Magic3D
{
    
    
/** Represent the master interface to the event system
 */
class EventSystem
{
private:
    static bool one;

public:
    /// standard constructor
    inline EventSystem()
    {
        if ( one )
            throw_MagicException("The event system currently only supports one instance." );
        one = true;
    }

    /// destructor
    inline ~EventSystem() 
    {
        one = false;
    }
    
    /// initialize
    void init();
    
    /// deinitalize
    void deinit();
    

    inline bool poll( Event* event)
    {   
        SDL_Event e;
        while ( SDL_PollEvent( &e ) )
        {
            if ( event->set(e) )
                return true;
        }
        return false;
    }

};



};


#endif








