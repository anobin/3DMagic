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
/** Header file for World class
 *
 * @file World.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_WORLD_H
#define MAGIC3D_WORLD_H

#include "../Cameras/Camera.h"
#include "../Graphics/GraphicsSystem.h"
#include "../Physics/PhysicsSystem.h"
#include "../Objects/Object.h"
#include "../Time/StopWatch.h"

#include <set>


namespace Magic3D
{


/** Serves to manage the world state for a virtual environment and provide 
 * access to the graphical, physical, and audio systems comprising the
 * environment.
 */
class World
{
private:
    std::set<Object*> objects;
    
    GraphicsSystem& graphics;
    
    PhysicsSystem& physics;
    
    StopWatch frameTimer;
    
    int fps;
    
    float physicsStepTime;
    
    bool alignPStep2FPS;
    
    int physicsStepsPerFrame;
    
    int actualFPS;
    
    Camera* camera;
    
public:
    inline World( GraphicsSystem* graphics, PhysicsSystem* physics):
        graphics(*graphics), physics(*physics), fps(60), physicsStepTime(1.0f/60.0f),
        alignPStep2FPS(true), physicsStepsPerFrame(1), actualFPS(0), camera(NULL) {} 
    
   inline void addObject(Object* object)
   {
       objects.insert( object );
       if (object->getPhysical())
           physics.addBody( *object->getPhysical() );
   }
   
   inline void removeObject(Object* object)
   {
       std::set<Object*>::iterator it = objects.find(object);
       if ( it != objects.end() )
       {
           objects.erase(it);
           if (object->getPhysical())
               physics.removeBody(*object->getPhysical());
       }
   }
   
   inline void setCamera(Camera* camera)
   {
       this->camera = camera;
   }
   
   inline void setTargetFPS(int fps)
   {
       this->fps = fps;
       if ( alignPStep2FPS )
           physicsStepTime = 1.0f/((float)fps);
   }
   
   inline void alignPhysicsStepToFPS( bool align )
   {
       this->alignPStep2FPS = align;
       if (align)
       {
           this->physicsStepsPerFrame = 1;
           physicsStepTime = 1.0f/((float)fps);
       }
   }
   
   inline void setPhysicsStepTime( float time )
   {
       if (alignPStep2FPS)
       {
           throw_MagicException( "Tried to manually set physics step time when physics "
               "step is aligned to FPS." );
       }
       this->physicsStepTime = time;
   }
   
   inline void setPhysicsStepsPerFrame( int steps )
   {
       if (alignPStep2FPS)
       {
           throw_MagicException( "Tried to manually set physics steps per frame when physics "
               "step is aligned to FPS." );
       }
       this->physicsStepsPerFrame = steps;
   }
   
   inline void startFrame()
   {
       frameTimer.reset();
   }
   
   virtual void processFrame();
   
   inline void endFrame()
   {
       float frameTime = 1.0f/((float)fps);
       while (frameTimer.getElapsedTime() < frameTime);
       actualFPS = (int) (1.0f / frameTimer.getElapsedTime());
   }
   
   inline int getActualFPS()
   {
       return actualFPS;
   }
    
};

};











#endif
