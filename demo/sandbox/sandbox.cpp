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

// textures
Texture* stoneTex = NULL;
Texture* marbleTex = NULL;
Texture* bunkerTex = NULL;
Texture* brickTex = NULL;
Texture* blueTex = NULL;

// models
Model floorModel;
Model sphereModel;
Model tinySphereModel;
Model bigSphereModel;
Model boxModel;

// meshes
Mesh floorMesh;
Mesh sphereMesh;
Mesh tinySphereMesh;
Mesh bigSphereMesh;
Mesh boxMesh;

// materials
Material floorMaterial;
Material sphereMaterial;
Material tinySphereMaterial;
Material bigSphereMaterial;
Material boxMaterial; 

// objects
Object* bigBall;
Object* laser;
Object* floorObject;
Object* ceiling;
Object* wallObject;

// shader uniforms
float lightPos[] = {0.0f, 1000.0f, 0.0f};
Color groundColor(25,25,25);
Color skyColor(255,255,255);

// shaders
Shader* shader;

// cameras
FPCamera camera;

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

// builders
MeshBuilder meshBuilder;
MaterialBuilder materialBuilder;
ModelBuilder modelBuilder;

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
	
	// init shader
	Handle<TextResource> vp = resourceManager.get<TextResource>("shaders/HemisphereTexShader.vp");
    Handle<TextResource> fp = resourceManager.get<TextResource>("shaders/HemisphereTexShader.fp");
	shader = new Shader( vp()->getText(), fp()->getText() );
	shader->bindAttrib( "vertex" );
	shader->bindAttrib( "normal" );
	shader->bindAttrib( "texcoord0" );
	shader->link();
	
	
	// init meshes
	meshBuilder.buildSphere(&sphereMesh, 2*FOOT, 55, 32);
	meshBuilder.buildSphere(&tinySphereMesh, 1*FOOT, 4, 4);
	meshBuilder.buildBox(&bigSphereMesh, 2, 4, 3);
	meshBuilder.buildFlatSurface(&floorMesh, ROOM_SIZE*50, ROOM_SIZE*50, 20, 20, 
	    true, 15*FOOT, 12*FOOT );
	float scale = 5.0f;
	meshBuilder.buildBox(&boxMesh, 6*INCH*scale, 3*INCH*scale, 3*INCH*scale );
	
	// init materials
	materialBuilder.begin(&sphereMaterial, 4, 3);
	materialBuilder.setShader(shader);
	materialBuilder.addAutoUniform( "mvMatrix", Material::MODEL_VIEW_MATRIX );
	materialBuilder.addAutoUniform( "mvpMatrix", Material::MODEL_VIEW_PROJECTION_MATRIX );
	materialBuilder.addAutoUniform( "normalMatrix", Material::NORMAL_MATRIX );
	materialBuilder.addAutoUniform( "textureMap", Material::TEXTURE0 );
	materialBuilder.addNamedUniform( "lightPosition", VertexArray::FLOAT, 3, lightPos);
	materialBuilder.addNamedUniform( "skyColor", VertexArray::FLOAT, 3, skyColor.getInternal());
	materialBuilder.addNamedUniform( "groundColor", VertexArray::FLOAT, 3, groundColor.getInternal());
	materialBuilder.setTexture(marbleTex);
	materialBuilder.end();
	materialBuilder.begin(&tinySphereMaterial, 4, 3);
	materialBuilder.setShader(shader);
	materialBuilder.addAutoUniform( "mvMatrix", Material::MODEL_VIEW_MATRIX );
	materialBuilder.addAutoUniform( "mvpMatrix", Material::MODEL_VIEW_PROJECTION_MATRIX );
	materialBuilder.addAutoUniform( "normalMatrix", Material::NORMAL_MATRIX );
	materialBuilder.addAutoUniform( "textureMap", Material::TEXTURE0 );
	materialBuilder.addNamedUniform( "lightPosition", VertexArray::FLOAT, 3, lightPos);
	materialBuilder.addNamedUniform( "skyColor", VertexArray::FLOAT, 3, skyColor.getInternal());
	materialBuilder.addNamedUniform( "groundColor", VertexArray::FLOAT, 3, groundColor.getInternal());
	materialBuilder.setTexture(blueTex);
	materialBuilder.end();
	materialBuilder.begin(&bigSphereMaterial, 4, 3);
	materialBuilder.setShader(shader);
	materialBuilder.addAutoUniform( "mvMatrix", Material::MODEL_VIEW_MATRIX );
	materialBuilder.addAutoUniform( "mvpMatrix", Material::MODEL_VIEW_PROJECTION_MATRIX );
	materialBuilder.addAutoUniform( "normalMatrix", Material::NORMAL_MATRIX );
	materialBuilder.addAutoUniform( "textureMap", Material::TEXTURE0 );
	materialBuilder.addNamedUniform( "lightPosition", VertexArray::FLOAT, 3, lightPos);
	materialBuilder.addNamedUniform( "skyColor", VertexArray::FLOAT, 3, skyColor.getInternal());
	materialBuilder.addNamedUniform( "groundColor", VertexArray::FLOAT, 3, groundColor.getInternal());
	materialBuilder.setTexture(blueTex);
	materialBuilder.end();
	materialBuilder.begin(&floorMaterial, 4, 3);
	materialBuilder.setShader(shader);
	materialBuilder.addAutoUniform( "mvMatrix", Material::MODEL_VIEW_MATRIX );
	materialBuilder.addAutoUniform( "mvpMatrix", Material::MODEL_VIEW_PROJECTION_MATRIX );
	materialBuilder.addAutoUniform( "normalMatrix", Material::NORMAL_MATRIX );
	materialBuilder.addAutoUniform( "textureMap", Material::TEXTURE0 );
	materialBuilder.addNamedUniform( "lightPosition", VertexArray::FLOAT, 3, lightPos);
	materialBuilder.addNamedUniform( "skyColor", VertexArray::FLOAT, 3, skyColor.getInternal());
	materialBuilder.addNamedUniform( "groundColor", VertexArray::FLOAT, 3, groundColor.getInternal());
	materialBuilder.setTexture(stoneTex);
	materialBuilder.end();
	materialBuilder.begin(&boxMaterial, 4, 3);
	materialBuilder.setShader(shader);
	materialBuilder.addAutoUniform( "mvMatrix", Material::MODEL_VIEW_MATRIX );
	materialBuilder.addAutoUniform( "mvpMatrix", Material::MODEL_VIEW_PROJECTION_MATRIX );
	materialBuilder.addAutoUniform( "normalMatrix", Material::NORMAL_MATRIX );
	materialBuilder.addAutoUniform( "textureMap", Material::TEXTURE0 );
	materialBuilder.addNamedUniform( "lightPosition", VertexArray::FLOAT, 3, lightPos);
	materialBuilder.addNamedUniform( "skyColor", VertexArray::FLOAT, 3, skyColor.getInternal());
	materialBuilder.addNamedUniform( "groundColor", VertexArray::FLOAT, 3, groundColor.getInternal());
	materialBuilder.setTexture(brickTex);
	materialBuilder.end();
	
    // init models
    modelBuilder.buildSimpleModel(&sphereModel, &sphereMesh, &sphereMaterial);
	modelBuilder.buildSimpleModel(&tinySphereModel, &tinySphereMesh, &tinySphereMaterial);
	modelBuilder.buildSimpleModel(&bigSphereModel, &bigSphereMesh, &bigSphereMaterial);
	modelBuilder.buildSimpleModel(&floorModel, &floorMesh, &floorMaterial);
	modelBuilder.buildSimpleModel(&boxModel, &boxMesh, &boxMaterial);
	
	// init objects
	btBall = new Object(&sphereModel, Point3(0.0f, 150*FOOT, 0.0f));
	world->addObject(btBall);
	
	floorObject = new Object(&floorModel); // static object
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
			btBox = new Object(&boxModel, Point3(w, h, zOffset) );
			world->addObject(btBox);
		}
	}
        
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
            Object* t = new Object(&tinySphereModel, Point3(0, 10.0f, 0) );
            world->addObject(t);
            
            //t->getPhysical().getRigidBody()->applyForce(btVector3(((float)(rand()%100))*0.01f, 0.0f, ((float)(rand()%100))*0.01f), 
            //                              btVector3(0.0f, 0.0f, 0.0f));
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
			t = new Object(&bigSphereModel, Point3(0.0f, 5.0f, 0.0f) );
			world->addObject(t);
			
			camera.lookat( Point3(0, 30, 0) );
			
			break;
		case 'h':
		    releaseWater = true;
			break;
		case 'p':
			if (paused)
			{
				paused = false;
				world->alignPhysicsStepToFPS(true);
			}
			else
			{
				paused = true;
				world->alignPhysicsStepToFPS(false);
				world->setPhysicsStepsPerFrame(0);
			}
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
	//static float speed = 700 * 300;
	
	switch(button)
	{
	    case Event::LEFT:
			p.set(camera.getPosition());
			p.translateLocal(0.0f, -1.5*FOOT, -2.0*FOOT);
			
			t = new Object(&sphereModel, p );
			world->addObject(t);
			//t->getPhysical().getRigidBody()->applyForce(btVector3(p.getForwardVector().getX()*speed, 
			//							p.getForwardVector().getY()*speed, p.getForwardVector().getZ()*speed), 
			//							  btVector3(0.0f, 0.0f, 0.0f));
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


































