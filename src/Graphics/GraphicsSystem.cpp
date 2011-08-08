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
/** Implementation file for GraphicsSystem class
 *
 * @file GraphicsSystem.cpp
 * @author Andrew Keating
 */
 
#include <Graphics/GraphicsSystem.h>



namespace Magic3D
{
    
extern int init_SDL(unsigned int flags);
extern void quit_SDL(unsigned int flags);
    
bool GraphicsSystem::one = false;
    
/// initialize
void GraphicsSystem::init()
{
    if (initialized)
        throw_MagicException( "Attempt to initalize a graphics system twice" );
    
    if ( init_SDL( SDL_INIT_VIDEO ) < 0 ) 
        throw_MagicException ( "Failed to initalize grpahics system (SDL)" );
    
    // becuase GLEW needs a display 
    initialized = true;
    this->createScreen();
    initialized = false;
    
    // init GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
        throw_MagicException ( "Failed to initalize graphics system (GLEW)" );
    initialized = true;
}

/// deinitalize
void GraphicsSystem::deinit()
{
    quit_SDL(SDL_INIT_VIDEO);
}
    

/// create the display screen
void GraphicsSystem::createScreen()
{
    if (!initialized)
    {
        throw_MagicException( "Attempt to create display screen before grpahics system"
            " is initialized" );
    }
    
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,      8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

    screen = SDL_SetVideoMode(displayWidth, displayHeight, 32, 
        SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE | SDL_OPENGL );
    if ( screen == NULL )
        throw_MagicException( "Failed to create display screen" );
    glViewport(0, 0, displayWidth, displayHeight);
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
};








