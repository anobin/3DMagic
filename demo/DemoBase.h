/* 
Copyright (c) 2015 Andrew Keating

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

#ifndef MAGIC3D_DEMO_BASE_H
#define MAGIC3D_DEMO_BASE_H

#include <3DMagic.h>
using namespace Magic3D;

class DemoBase
{
private:
	void changeWindowSize(int w, int h)
	{
		// Create the projection matrix
		camera.setPerspectiveProjection(60.0f, float(w)/float(h), INCH, 1000*FOOT);
	}

protected:
	World* world;
	GraphicsSystem graphics;
	PhysicsSystem physics;
	EventSystem events;
	FPCamera camera;
	Position lightPos;

public:

	inline DemoBase(): lightPos(0.0f, 1000.0f, 0.0f)
	{
		graphics.init();
		graphics.setDisplaySize( 1280, 1024 );
		graphics.createScreen();
	
		physics.init();
	
		world = new World(&graphics, &physics);
		world->setCamera(&camera);
		world->setLight(&lightPos);

		events.init();
	}

	virtual ~DemoBase()
	{
		delete world;
	}

	void start()
	{
		changeWindowSize(1280,1024); // to ensure transform pipeline is setup
		graphics.createScreen();
	
		// run SDL
		Event event;
		boolean stop = false;
		while( true )
		{
			world->startFrame();
	    
			while ( events.poll( &event ) )
			{
				switch( event.data.type )
				{
					case Event::QUIT:
						stop = true;
						break;
	                
					case Event::VIDEO_RESIZE:
						graphics.setDisplaySize( event.data.resize.w, event.data.resize.h );
						graphics.createScreen();
						changeWindowSize( event.data.resize.w, event.data.resize.h );
						break;
				}
				this->handleEvent(event);
			}
			if (stop)
				break;
	        
			this->tick();
	    
			world->stepPhysics();
	    
			world->renderObjects();
	    
			world->endFrame();
		}

		this->stop();
	}

	void stop()
	{
		graphics.deinit();
		physics.deinit();
		events.deinit();
	}

	virtual void tick(void) = 0;

	virtual void handleEvent(const Event& event) = 0;

};


#endif