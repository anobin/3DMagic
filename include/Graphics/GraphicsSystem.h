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
/** Header file for GraphicsSystem class
 *
 * @file GraphicsSystem.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_GRAPHICS_SYSTEM_H
#define MAGIC3D_GRPAHICS_SYSTEM_H

// SDL includes
#include <SDL/SDL.h>

#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif


#include "../Exceptions/MagicException.h"
#include "../Util/Color.h"

namespace Magic3D
{
    
    
/** Represent the master interface to the graphics system
 */
class GraphicsSystem
{
private:
    static bool one;
    
    bool initialized;
    
    /// SDL screen surface, do not try to ever free
    SDL_Surface *screen;
    
    int displayWidth;
    
    int displayHeight;

public:
    /// standard constructor
    inline GraphicsSystem(): initialized(false), screen(NULL), displayWidth(640),
        displayHeight(480) 
    {
        if ( one )
            throw_MagicException("The graphics system currently only supports one instance." );
        one = true;
    }

    /// destructor
    inline ~GraphicsSystem() 
    {
        one = false;
    }
    
    /// initialize
    void init();
    
    /// deinitalize
    void deinit();
    
    inline void setDisplaySize( int width, int height )
    {
        this->displayWidth = width;
        this->displayHeight = height;
    }
    
    /// create the display screen
    void createScreen();
    
    inline void showCursor( bool show )
    {
        if (show )
            SDL_ShowCursor( SDL_ENABLE );
        else
            SDL_ShowCursor( SDL_DISABLE );
    }
    
    inline void warpMouse( int x, int y )
    {
        SDL_WarpMouse(x, y);
    }

    inline void swapBuffers()
    {
        SDL_GL_SwapBuffers();
    }
    
    inline void clearDisplay()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    inline void enableBlending()
    {
        glEnable (GL_BLEND); 
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    inline void setDepthOffset(float offset )
    {
        glPolygonOffset(offset, offset);   
        glEnable(GL_POLYGON_OFFSET_FILL);
    }
    
    inline void disableDepthOffset()
    {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
    
    inline void enableDepthTest()
    {
        glEnable(GL_DEPTH_TEST);
    }
    
    inline void disableDepthTest()
    {
        glDisable(GL_DEPTH_TEST);
    }
    
    inline void setClearColor( const Color& color )
    {
        const GLfloat* c = color.getInternal();
        glClearColor(c[0], c[1], c[2], c[3]);
    }

};



};


#endif








