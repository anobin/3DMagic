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
/** Implementation file for EventSystem class
 *
 * @file EventSystem.cpp
 * @author Andrew Keating
 */
 
#include <Event/EventSystem.h>



namespace Magic3D
{
    
extern int init_SDL(unsigned int flags);
extern void quit_SDL(unsigned int flags);
    
bool EventSystem::one = false;
    
/// initialize
void EventSystem::init()
{   
    if ( init_SDL(0) < 0 ) 
        throw_MagicException ( "Failed to initalize event system (SDL)" );
}

/// deinitalize
void EventSystem::deinit()
{
    quit_SDL(0); // no SDL subsystem for events
}
    
    
    
    
    
    
    
    
    
    
    
    
    
};








