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
/** Implementation file for World class
 *
 * @file World.h
 * @author Andrew Keating
 */
 
#include <World/World.h>
#include <Renderers/Renderer.h>


namespace Magic3D
{

void World::processFrame()
{
    std::set<Renderer*> renderers;
    std::set<Object*> transObjects;
    
    // step physics
    if ( physicsStepTime > 0.0f )
    {
        static float normalStepSize = 1.0f/60.0f;
        int subSteps = (int) (physicsStepTime / normalStepSize);
        if (subSteps < 1 )
            subSteps = 1;
        
        for (int i=0; i < physicsStepsPerFrame; i++)
            physics.stepSimulation(physicsStepTime, subSteps);
    }
    
    // Clear the color and depth buffers
	graphics.clearDisplay();

	// render all objects
	std::set<Object*>::iterator it = objects.begin();
	std::set<Renderer*>::iterator rit;
	Renderer* ren;
	Object* ob;
	int passCount;
	bool check = true;
	for(; ; it++)
	{
	    if (check && it == objects.end())
	    {
	        it = transObjects.begin();
	        if (it == transObjects.end())
	            break;
	        check = false;
	    }
        else if (!check && it == transObjects.end())
            break;
	    
	    ob = (*it);
	    ren = ob->getGraphical().getRenderer();
	    passCount = ren->getPassCount();
	    
	    // check for transparency
	    bool t = false;
	    std::vector< VertexBatch* >& batches = ob->getGraphical().getModel().getBatchData();
	    std::vector< VertexBatch* >::iterator batch = batches.begin();
	    for(; batch != batches.end() && check; batch++)
	    {
	        if ( (*batch)->isPropertySet(VertexBatch::TRANSPARENCY) && 
	             (*batch)->getProperty<bool>(VertexBatch::TRANSPARENCY) )
	        {
	            transObjects.insert(ob);
	            t = true;
	            break;
	        }
	    }
	    if (t)
	        continue;
	    
	    // check if the renderer has been setup
	    rit = renderers.find(ren);
	    if (rit == renderers.end())
	    {
	        ren->setup(0);
	        renderers.insert(ren);
	    }
	    
	    // render object
	    ren->render( ob, 0 );
	}
	
	// Do the buffer Swap
    graphics.swapBuffers();
}










};


















