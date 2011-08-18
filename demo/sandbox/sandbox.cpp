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
/** Generates a 3D environment used to test different features
 */

// 3DMagic includes
#include <3DMagic.h>
using namespace Magic3D;

// SDL includes
#include <SDL/SDL.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <map>
#include <list>
using std::cout;
using std::endl;
         

#define FPS2MS(x) (1000 / (x))
#define RGB_SINGLE(x) ((1.0f/255.0f)*(x))
#define RGB(r, g, b) {RGB_SINGLE((r)), RGB_SINGLE((g)), RGB_SINGLE((b)), 1.0f}

#define ROOM_SIZE (20.0f * FOOT)


Matrix4 projectionMatrix;

// resource manager
ResourceManager resourceManager("../../");

// renderers
Renderer2D* renderer2D;
HemRenderer* renderer;

// textures
Texture* stoneTex = NULL;
Texture* marbleTex = NULL;
Texture* bunkerTex = NULL;
Texture* brickTex = NULL;
Texture* blueTex = NULL;

// models
FlatSurface* ceilingModel;
FlatSurface* floorModel;
FlatSurface* wallModel;
Sphere* sphere;
Sphere* tinySphere;
Box* bigSphere;
Box* boxModel;

// objects
Object* bigBall;
Object* laser;
Object* floorObject;
Object* ceiling;
Object* wallObject;

// decal stuff
Object* decal;
FlatSurface* decalSurface;
std::vector<Object*> decals;
std::map<Object*, Object*> relations;

// 3ds test
CustomModel* chainLinkModel;
Texture* chainLinkTex;

// GUI stuff
Rectangle2D* frameModel;
Circle2D* circleModel;
Texture* frameTex;
Texture* circleTex;
Object* frameObject;
Object* circleObject;
Matrix4 flatProjectionMatrix;

// legacy
//Position             cameraFrame;
Point3 lightPos(0.0f, 1000.0f, 0.0f);
Color groundColor(25,25,25);
Color skyColor(255,255,255);

FPCamera camera;
Camera2D camera2d;

bool wireframe = false;

int screenWidth = 0;
int screenHeight = 0;

// tracks game time
StopWatch	timer;
StopWatch   physicsTimer;

bool lockCursor = false;
bool moveForward = false;
bool moveBack = false;
bool moveLeft = false;
bool moveRight = false;
bool releaseWater = false;

// world and systems
GraphicsSystem graphics;
PhysicsSystem physics;
EventSystem events;
World* world;

/** Called when the window size changes
 * @param w width of the new window
 * @param h hieght of the new window
 */
void changeWindowSize(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	
    // Create the projection matrix
    camera.setPerspectiveProjection(60.0f, float(w)/float(h), INCH, 1000*FOOT);
	
	// create the orthographic/flat project matrix for the screen size and -1 to 1 depth
	camera2d.setOrthographicProjection(0, w, 0, h, -1.0, 1.0);
	
	frameModel = new Rectangle2D(w-173-10, 10, 173, 50);
}


Object* btBall; // graphical presence of ball used for bullet
Object* btBox;

/** Global setup-related steps are performed here
 */
void setup()
{	
	// bullet setup
	physics.setGravity(0,-9.8*METER,0);
	
	graphics.enableDepthTest();
	
    static Color lightBlue(5, 230, 255, Color::RGB_BYTE);
	graphics.setClearColor(lightBlue );
	
	// init textures
	if (resourceManager.doesResourceExist("images/bareConcrete.tga"))
	{
		Handle<TGA2DResource> stoneImage = resourceManager.get<TGA2DResource>("images/bareConcrete.tga");
		stoneTex = new Texture(stoneImage());
	}
	else
	{
		Handle<SingleColor2DResource> stoneImage = resourceManager.injectSingleColor2D(
				"images/bareConcrete.tga", Color::WHITE);
		stoneTex = new Texture(stoneImage());
	}
	if (resourceManager.doesResourceExist("images/marble.png"))
	{
		Handle<PNG2DResource> marbleImage = resourceManager.get<PNG2DResource>("images/marble.png");
		marbleTex= new Texture(marbleImage());
		marbleTex->setWrapMode(Texture::CLAMP_TO_EDGE);
	}
	else
	{
		Handle<SingleColor2DResource> marbleImage = resourceManager.injectSingleColor2D(
				"images/marble.tga", Color::RED);
		marbleTex= new Texture(marbleImage());
		marbleTex->setWrapMode(Texture::CLAMP_TO_EDGE);
	}
	if (resourceManager.doesResourceExist("images/ConcreteBunkerDirty.tga"))
	{
		Handle<TGA2DResource> bunkerImage = resourceManager.get<TGA2DResource>("images/ConcreteBunkerDirty.tga");
		bunkerTex = new Texture(bunkerImage());
	}
	if (resourceManager.doesResourceExist("images/singleBrick.tga"))
	{
		Handle<TGA2DResource> brickImage = resourceManager.get<TGA2DResource>("images/singleBrick.tga");
		brickTex = new Texture(brickImage());
		brickTex->setWrapMode(Texture::CLAMP_TO_EDGE);
	}
	else
	{
		Handle<SingleColor2DResource> brickImage = resourceManager.injectSingleColor2D(
					"images/singleBrick.tga", Color(255, 118, 27, 255, Color::RGBA_BYTE));
		brickTex = new Texture(brickImage());
		brickTex->setWrapMode(Texture::CLAMP_TO_EDGE);
	}
	Handle<SingleColor2DResource> blueImage = resourceManager.injectSingleColor2D(
					"images/blue.tga", Color(31, 97, 240, 255, Color::RGBA_BYTE));
	blueTex = new Texture(blueImage());
	blueTex->setWrapMode(Texture::CLAMP_TO_EDGE);
	
	Handle<SingleColor2DResource> frameImage = resourceManager.injectSingleColor2D(
					"images/frameImage", Color(255, 118, 27, 100, Color::RGBA_BYTE));
	if (resourceManager.doesResourceExist("images/splatter.tga"))
	{
		Handle<TGA2DResource> splatterImage = resourceManager.get
			<TGA2DResource>("images/splatter.tga");
		circleTex = new Texture(splatterImage());
	}
	else
	{
		circleTex = new Texture(frameImage());
	}
	if (resourceManager.doesResourceExist("images/logo.png"))
	{
		Handle<PNG2DResource> logoImage = resourceManager.get
			<PNG2DResource>("images/logo.png");
		frameTex = new Texture(logoImage());
	}
	else
	{
		frameTex = new Texture(frameImage());
	}
	
	
    // init models, represent data on graphics card
	sphere = new Sphere(2*FOOT, 55, 32);
	sphere->setTexture(marbleTex);
	tinySphere = new Sphere( 1*FOOT, 4, 4);
	tinySphere->setTexture(blueTex);
	//bigSphere = new Sphere(50*FOOT, 55, 32);
	bigSphere = new Box(2, 4, 3);
	bigSphere->setTexture(marbleTex);
	ceilingModel = new FlatSurface(ROOM_SIZE*2, ROOM_SIZE*2, 20, 20, true, 15*FOOT, 12*FOOT);
	floorModel = new FlatSurface(ROOM_SIZE*50, ROOM_SIZE*50, 20, 20, true, 15*FOOT, 12*FOOT);
	floorModel->setTexture(stoneTex);
	wallModel = new FlatSurface(5*FOOT, 5*FOOT, 20, 20);
	float scale = 5.0f;
	boxModel = new Box(6*INCH*scale, 3*INCH*scale, 3*INCH*scale);
	boxModel->setFrontTexture(brickTex);
	boxModel->setBackTexture(brickTex);
	boxModel->setTopTexture(marbleTex);
	boxModel->setBottomTexture(blueTex);
	boxModel->setLeftTexture(stoneTex);
	boxModel->setRightTexture(blueTex);
	
	// init renderer
	renderer = new HemRenderer(resourceManager);
	renderer->setCamera( &camera );
	renderer->setLightSource( lightPos );
	renderer->setSkyColor( skyColor );
	renderer->setGroundColor( groundColor );
	
	// init 2D renderer
	renderer2D = new Renderer2D(resourceManager);
	renderer2D->setCamera( &camera2d );
	
	// init objects
	btBall = new Object(*sphere, 1, Point3(0.0f, 150*FOOT, 0.0f), renderer); // 1 kg sphere
	
	world->addObject(btBall);
	
	floorObject = new Object(*floorModel, 0, renderer); // static object
	world->addObject(floorObject);
	
	float wallWidth =40;
	float wallHeight = 10;
	float brickHeight = 3*INCH*scale;
	float brickWidth = 6*INCH*scale;
	float h = brickHeight/2;
	float xOffset = -(brickWidth*wallWidth)/2;
	float zOffset = -100*FOOT;
	for (int i=0; i < wallHeight; i++, h+=brickHeight)
	{
		float w = xOffset;
		if (i%2 != 0)
			w = brickWidth/2 + xOffset;
		for (int j=0; j < wallWidth; j++, w+=brickWidth)
		{
			if (i == wallHeight-1 && j == wallWidth-1)
				continue;
			btBox = new Object(*boxModel, 4, Point3(w, h, zOffset)/*, 3.0f*/, renderer); // 3 kg box
			world->addObject(btBox);
		}
	}
	
	// gui stuff
	frameModel = new Rectangle2D(10, 10, 173, 50);
	circleModel = new Circle2D(400, 60, 100, 1.0f);
	
	
	frameModel->setTexture(frameTex);
	frameModel->setTransparency(true);
	circleModel->setTexture(circleTex);
	circleModel->setTransparency(true);
	
	frameObject = new Object( *frameModel, 0, renderer2D);
	world->addObject( frameObject );
	circleObject = new Object( *circleModel, 0, renderer2D);
	world->addObject( circleObject );
	
	// 3ds test stuff
	Handle<Model3DSResource> linkResource = 
		resourceManager.get<Model3DSResource>("models/chainLink.3ds");
	chainLinkModel = new CustomModel(*linkResource());
	Handle<SingleColor2DResource> linkImage = resourceManager.injectSingleColor2D(
					"images/linkImage", Color(195, 195, 195, 255, Color::RGBA_BYTE));
	chainLinkTex = new Texture(linkImage());
	chainLinkModel->setTexture(chainLinkTex);
    
    // decal stuff
    decalSurface = new FlatSurface(6*FOOT, 6*FOOT, 20, 20, true, 6*FOOT, 6*FOOT );
    decal = new Object( *decalSurface, 0, Point3( 0.0f, 3*FOOT, -2*FOOT ), renderer );
    decal->getPosition().rotate( 90.0f / 180.0f * M_PI,  Vector3( 1.0f, 0.0f, 0.0f ) );
	decals.push_back(decal);
    
    Object* chainLink = new Object( *chainLinkModel, 3, Point3(0.0f, 3.0f*FOOT, 0.0f), renderer );
    world->addObject(chainLink);
	
        
    // set eye level
    camera.setLocation(Point3(0.0f, 6 * FOOT, ROOM_SIZE));
	camera.setStepSpeed( FOOT );
	camera.setStrafeSpeed( FOOT );
	
	// enable blending so transparency can happen
	graphics.enableBlending();
	
	srand(time(NULL));
}


bool paused = true;
int slow = 0;
/** Called to render each and every frame
 */
void renderScene(void)
{	
	// move
	Vector3 side;
	if (moveForward)
	    camera.step(1);
	if (moveBack)
		camera.step(-1);
	if (moveLeft)
	    camera.strafe(1);
	if (moveRight)
	    camera.strafe(-1);
	
	// release water
	if (releaseWater)
	{
	    for (int i = 0; i < 20; i++)
        {
            Object* t = new Object(*tinySphere, 0.01f, Point3(/*1.1f * (i%5)*/0, 10.0f, /*1.1f * (i/5)*/0), renderer); // 0.01 kg sphere
            world->addObject(t);
            
            t->getPhysical().getRigidBody()->applyForce(btVector3(((float)(rand()%100))*0.01f, 0.0f, ((float)(rand()%100))*0.01f), 
                                          btVector3(0.0f, 0.0f, 0.0f));
        }
    }
    
}

/** Called when a normal key is pressed on the keyboard
 * @param key the key pressed
 * @param x the x-coord of the mouse at the time of the press
 * @param y the y-coord of the mouse at the time of the press
 */
void keyPressed(int key)
{
	Point3 origin;
	Vector3 forward;
	Vector3 side;
	Vector3 up;
	btTransform transform;
	Position p;
	Object* t;
    Object* decal;
    std::vector<Object*>::iterator it;
    //int i;
	
	
	
	switch(key)
	{
		// space
		case ' ':
			/*if (wireframe)
				wireframe = false;
			else
				wireframe = true;*/
			camera.setLocation( Point3(0.0f, 6.0f * FOOT, 20.0f * FOOT) );
			//camera.getPosition().getForwardVector().set(0.0f, 0.0f, -1.0f);
			//camera.getPosition().getUpVector().set(0.0f, 1.0f, 0.0f);
			
			// manually set new position for ball
			btBall->getPosition().getLocation().setY(10*FOOT);
			btBall->syncPositionToPhysics();
			break;
			
		// escape
		case 0x1B:
			exit(1);
			
		// w, forward
		case 'w':
			//cameraFrame.translate(cameraFrame.getForwardVector().getX()*FOOT, 0.0f, 
			//					  cameraFrame.getForwardVector().getZ()*FOOT);
			moveForward = true;
			break;
			
		// s, backward
		case 's':
			//cameraFrame.translate(-cameraFrame.getForwardVector().getX()*FOOT, 0.0f, 
			//					  -cameraFrame.getForwardVector().getZ()*FOOT);
			moveBack = true;
			break;
			
		// a, strafe left
		case 'a':
			//cameraFrame.getLocalXAxis(side);
			//cameraFrame.translate(side.getX()*FOOT, 0.0f, side.getZ()*FOOT);
			moveLeft = true;
			break;
			
		// d, strafe right
		case 'd':
			// can only move in the xz plane
			//cameraFrame.getLocalXAxis(side);
			//cameraFrame.translate(-side.getX()*FOOT, 0.0f, -side.getZ()*FOOT);
			moveRight = true;
			break;
			
		case '-':
			camera.elevate( -3*FOOT );
			break;
			
		case '=':
			camera.elevate( 3*FOOT );
			break;
			
		case 'g':
			t = new Object(*bigSphere, 300, Point3(0.0f, 5.0f, 0.0f), renderer); // 1 kg sphere
			world->addObject(t);
			
			camera.lookat( Point3(0, 30, 0) );
            
            // add decal
            decal = new Object( *decalSurface, 0, Point3( 0.0f, 0.0f, 0.0f ), renderer);
            decals.push_back(decal);
            relations.insert( std::pair<Object*,Object*>( decal, t ) );
			break;
		case 'h':
		    releaseWater = true;
			break;
		case 'p':
			if (paused)
				paused = false;
			else
				paused = true;
			break;
			
		case 'z':
			break;
			
		case 'u':
		    if (lockCursor)
		    {
		        graphics.showCursor( true );
		        lockCursor = false;
		    }
		    else
		    {
		        graphics.warpMouse(screenWidth / 2, screenHeight / 2);
		        graphics.showCursor( false );
		        lockCursor = true;
		    }
		    break;
		    
		case ',':
		    if (slow != 0)
		        slow--;
		    cout << "physics speed is " << slow << "x" << endl;
		    break;
		    
		case '.':
		    slow++;
		    cout << "physics is " << slow << "x" << endl;
		    break;

        default:
            break;
        
	}
	
}

/** Called when a normal key is pressed on the keyboard
 * @param key the key pressed
 * @param x the x-coord of the mouse at the time of the press
 * @param y the y-coord of the mouse at the time of the press
 */
void keyReleased(int key)
{	
	
	switch(key)
	{
			
		// w, forward
		case 'w':
			//cameraFrame.translate(cameraFrame.getForwardVector().getX()*FOOT, 0.0f, 
			//					  cameraFrame.getForwardVector().getZ()*FOOT);
			moveForward = false;
			break;
			
		// s, backward
		case 's':
			//cameraFrame.translate(-cameraFrame.getForwardVector().getX()*FOOT, 0.0f, 
			//					  -cameraFrame.getForwardVector().getZ()*FOOT);
			moveBack = false;
			break;
			
		// a, strafe left
		case 'a':
			//cameraFrame.getLocalXAxis(side);
			//cameraFrame.translate(side.getX()*FOOT, 0.0f, side.getZ()*FOOT);
			moveLeft = false;
			break;
			
		// d, strafe right
		case 'd':
			// can only move in the xz plane
			//cameraFrame.getLocalXAxis(side);
			//cameraFrame.translate(-side.getX()*FOOT, 0.0f, -side.getZ()*FOOT);
			moveRight = false;
			break;
			
		case 'h':
		    releaseWater = false;
		    break;
        
	}
	
}

/** Called when a special key is pressed on the keyboard
 * @param key the key pressed
 * @param x the x-coord of the mouse at the time of the press
 * @param y the y-coord of the mouse at the time of the press
 */
void specialKeyPressed(int key, int x, int y)
{
	
}


/** Called when the mouse is clicked
 * @param button the button on the mouse that was clicked (GLUT_LEFT_BUTTON,GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON)
 * @param state either GLUT_UP or GLUT_DOWN
 * @param x the x-coord of the mouse at the time of the press
 * @param y the y-coord of the mouse at the time of the press
 */
void mouseClicked(Event::MouseButtons button, int x, int y)
{
	
	Position p;
	Object* t;
	static float speed = 700 * 300;
	
	switch(button)
	{
	    case Event::LEFT:
			p.set(camera.getPosition());
			p.translateLocal(0.0f, -1.5*FOOT, -2.0*FOOT);
			
			t = new Object(*sphere, 100, p, renderer); // 1 kg sphere
			world->addObject(t);
			t->getPhysical().getRigidBody()->applyForce(btVector3(p.getForwardVector().getX()*speed, 
										p.getForwardVector().getY()*speed, p.getForwardVector().getZ()*speed), 
										  btVector3(0.0f, 0.0f, 0.0f));
			break;
			
		case Event::MIDDLE: 
        case Event::RIGHT: 
        case Event::WHEEL_UP: 
        case Event::WHEEL_DOWN:
            break;
	}
}

/** Called when the mouse is moved with a button pressed
 * @param x the x-coord of the mouse pointer
 * @param y the y-coord of the mouse pointer
 */
void mouseMoved(int x, int y)
{
	
}


#define Y_AXIS_SENSITIVITY 0.3f
#define X_AXIS_SENSITIVITY 0.3f

/** Called when the mouse is moved without a button pressed
 * @param x the x-coord of the mouse pointer
 * @param y the y-coord of the mouse pointer
 */
void mouseMovedPassive(int x, int y)
{
    if (!lockCursor)
        return;
    
	// avoid reprocess from warp pointer call
	if (x == (screenWidth/2) && y == (screenHeight/2))
		return;
	
	camera.panView( -(x - (screenWidth/2))  * X_AXIS_SENSITIVITY, 
	                 (y - (screenHeight/2)) * Y_AXIS_SENSITIVITY 
	              );

	graphics.warpMouse(screenWidth / 2, screenHeight / 2);
}


/** Main program entry point
 */
int main(int argc, char* argv[])
{
	
	graphics.init();
	graphics.setDisplaySize( 1280, 1024 );
	graphics.createScreen();
	
	physics.init();
	
	world = new World(&graphics, &physics);
	
	events.init();
	
	// perform setup
	setup();
	
	changeWindowSize(1280,1024); // to ensure transform pipeline is setup
	graphics.createScreen();
	
	// run SDL
	Event event;
	while( true )
	{
	    world->startFrame();
	    
	    while ( events.poll( &event ) )
	    {
	        switch( event.data.type )
	        {
	            case Event::QUIT:
	                exit(0);
	                
	            case Event::VIDEO_RESIZE:
	                graphics.setDisplaySize( event.data.resize.w, event.data.resize.h );
	                graphics.createScreen();
	                changeWindowSize( event.data.resize.w, event.data.resize.h );
	                break;
	                
	            case Event::KEY_DOWN:
	                keyPressed( event.data.key.key );
	                break;
	                
	            case Event::MOUSE_MOTION:
	                mouseMovedPassive( event.data.motion.x, event.data.motion.y );
	                break;
	                
	            case Event::MOUSE_BUTTON_DOWN:
	                mouseClicked( event.data.button.button, event.data.button.x, event.data.button.y );
	                break;
	                
	            case Event::MOUSE_BUTTON_UP:
	                break;
	                
	            case Event::KEY_UP:
	                keyReleased( event.data.key.key );
	                break;
	        }
	    }
	        
	    renderScene();
	    
	    world->processFrame();
	    
	    world->endFrame();
	}
	
	graphics.deinit();
	physics.deinit();
	events.deinit();
	
    
	return 0;
}


































