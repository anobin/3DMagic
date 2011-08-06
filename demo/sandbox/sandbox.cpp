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

// include Bullet physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

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

// models
FlatSurface* ceilingModel;
FlatSurface* floorModel;
FlatSurface* wallModel;
Sphere* sphere;
Box* bigSphere;
Box* boxModel;

// objects
Object* bigBall;
Object* laser;
Object* floorObject;
Object* ceiling;
Object* wall[4];
std::vector<Object*> objects;

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
std::vector<Object*> objects2d;
Matrix4 flatProjectionMatrix;

// legacy
Position             cameraFrame;
Point3 lightPos(0.0f, 1000.0f, 0.0f);
Color groundColor(25,25,25);
Color skyColor(255,255,255);

bool wireframe = false;

int screenWidth = 0;
int screenHeight = 0;

// tracks game time
StopWatch	timer;
StopWatch   physicsTimer;

// world and systems
GraphicsSystem graphics;

/** Called when the window size changes
 * @param w width of the new window
 * @param h hieght of the new window
 */
void changeWindowSize(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	
    // Create the projection matrix
    projectionMatrix.createPerspectiveMatrix(60.0f, float(w)/float(h), INCH, 1000*FOOT);
	
	// create the orthographic/flat project matrix for the screen size and -1 to 1 depth
	flatProjectionMatrix.createOrthographicMatrix(0, w, 0, h, -1.0, 1.0);
	
	frameModel = new Rectangle2D(w-173-10, 10, 173, 50);
	
	if (renderer)
	    renderer->setProjectionMatrix( projectionMatrix );
	if (renderer2D)
	    renderer2D->setFlatProjectionMatrix( flatProjectionMatrix );
}


btDiscreteDynamicsWorld* dynamicsWorld;

Object* btBall; // graphical presence of ball used for bullet
Object* btBox;

/** Global setup-related steps are performed here
 */
void setup()
{	
	// bullet setup
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
 
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0,-9.8*METER,0));
	
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
	
	
    // init models, represent data on graphics card
	sphere = new Sphere(2*FOOT, 55, 32);
	//bigSphere = new Sphere(50*FOOT, 55, 32);
	bigSphere = new Box(2, 4, 3);
	ceilingModel = new FlatSurface(ROOM_SIZE*2, ROOM_SIZE*2, 20, 20, true, 15*FOOT, 12*FOOT);
	floorModel = new FlatSurface(ROOM_SIZE*50, ROOM_SIZE*50, 20, 20, true, 15*FOOT, 12*FOOT);
	wallModel = new FlatSurface(ROOM_SIZE*2, ROOM_SIZE, 20, 20);
	float scale = 5.0f;
	boxModel = new Box(6*INCH*scale, 3*INCH*scale, 3*INCH*scale);
	
	// init objects
	btBall = new Object(*sphere, 1, Point3(0.0f, 150*FOOT, 0.0f)); // 1 kg sphere
	btBall->getGraphical().setBaseTexture(*marbleTex);
	objects.push_back(btBall);
	//dynamicsWorld->addRigidBody(btBall->getRigidBody());
	
	floorObject = new Object(*floorModel, 0); // static object
	floorObject->getGraphical().setBaseTexture(*stoneTex);
	objects.push_back(floorObject);
	dynamicsWorld->addRigidBody(floorObject->getPhysical().getRigidBody());
	
	float wallWidth =60;
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
			btBox = new Object(*boxModel, 4, Point3(w, h, zOffset)/*, 3.0f*/); // 3 kg box
			btBox->getGraphical().setBaseTexture(*brickTex);
			objects.push_back(btBox);
			dynamicsWorld->addRigidBody(btBox->getPhysical().getRigidBody());
		}
	}
	

	
	
	
	/*ceiling.setModel(*ceilingModel);
	ceiling.setColor(Color::WHITE);
	ceiling.setBaseTexture(*marbleTex);
	ceiling.getPosition().setLocation(0.0f, ROOM_SIZE, 0.0f);
	//objects.push_back(&ceiling);
	
	for(int i=0; i < 4; i++)
	{
		wall[i].setModel(*wallModel);
		wall[i].setColor(Color::WHITE);
		wall[i].setBaseTexture(*bunkerTex);
		wall[i].getPosition().setLocation(0.0f, ROOM_SIZE/2, 0.0f);
		// move walls into place
		switch(i)
		{
			case 0:
				wall[i].getPosition().rotate(m3dDegToRad(90.0f), Vector3f(1.0f, 0.0f, 0.0f));
				wall[i].getPosition().translate(0.0f,0.0f, -ROOM_SIZE);
				break;
			case 1:
				wall[i].getPosition().rotate(m3dDegToRad(90.0f), Vector3f(1.0f, 0.0f, 0.0f));
				wall[i].getPosition().translate(0.0f,0.0f, ROOM_SIZE);
				wall[i].getPosition().rotate(m3dDegToRad(180.0f), Vector3f(0.0f, 1.0f, 0.0f));
				break;
			case 2:
				wall[i].getPosition().rotate(m3dDegToRad(90.0f), Vector3f(1.0f, 0.0f, 0.0f));
				wall[i].getPosition().rotate(m3dDegToRad(90.0f), Vector3f(0.0f, 1.0f, 0.0f));
				wall[i].getPosition().translate(ROOM_SIZE, 0.0f, 0.0f);
				wall[i].getPosition().rotate(m3dDegToRad(180.0f), Vector3f(0.0f, 1.0f, 0.0f));
				break;
			case 3:
				wall[i].getPosition().rotate(m3dDegToRad(90.0f), Vector3f(1.0f, 0.0f, 0.0f));
				wall[i].getPosition().rotate(m3dDegToRad(90.0f), Vector3f(0.0f, 1.0f, 0.0f));
				wall[i].getPosition().translate(-ROOM_SIZE, 0.0f, 0.0f);
				break;
		}
		//objects.push_back(&wall[i]);
	}*/
	
	
	// gui stuff
	frameModel = new Rectangle2D(10, 10, 173, 50);
	circleModel = new Circle2D(400, 60, 100, 1.0f);
	Handle<SingleColor2DResource> frameImage = resourceManager.injectSingleColor2D(
					"images/frameImage", Color(255, 118, 27, 100, Color::RGBA_BYTE));
	/*if (resourceManager.doesResourceExist("images/logo.tga"))
	{
		Handle<TGA2DResource> logoImage = resourceManager.get
			<TGA2DResource>("images/logo.tga");
		frameTex = new Texture(logoImage());
	}*/
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
	
	frameObject = new Object( *frameModel, 0);
	frameObject->getGraphical().setBaseTexture( *frameTex );
	objects2d.push_back( frameObject );
	circleObject = new Object( *circleModel, 0);
	circleObject->getGraphical().setBaseTexture( *circleTex );
	objects2d.push_back( circleObject );
	
	// 3ds test stuff
	Handle<Model3DSResource> linkResource = 
		resourceManager.get<Model3DSResource>("models/chainLink.3ds");
	chainLinkModel = new CustomModel(*linkResource());
	Handle<SingleColor2DResource> linkImage = resourceManager.injectSingleColor2D(
					"images/linkImage", Color(195, 195, 195, 255, Color::RGBA_BYTE));
	chainLinkTex = new Texture(linkImage());
    
    // decal stuff
    decalSurface = new FlatSurface(6*FOOT, 6*FOOT, 20, 20, true, 6*FOOT, 6*FOOT );
    decal = new Object( *decalSurface, 0, Point3( 0.0f, 3*FOOT, -2*FOOT ) );
    decal->getGraphical().setBaseTexture( *circleTex );
    decal->getPosition().rotate( 90.0f / 180.0f * M_PI,  Vector3( 1.0f, 0.0f, 0.0f ) );
	decals.push_back(decal);
	
	// chain link stuff
    /*Matrix4 scale;
    scale.createScaleMatrix(0.01, 0.01, 0.01);
    Position(0.0f, 3.0f*FOOT, 0.0f).getTransformMatrix(positionMatrix);
    Matrix4 transformMatrix;
    transformMatrix.multiply(cameraMatrix);
    transformMatrix.multiply(positionMatrix);
    transformMatrix.multiply(scale);
    scale.createRotationMatrix(90.0f*M_PI/180.0f, 1.0f, 0.0f, 0.0f);
    transformMatrix.multiply(scale);*/
    
    Matrix4 tempm;
    Object* chainLink = new Object( *chainLinkModel, 3, Point3(0.0f, 3.0f*FOOT, 0.0f) );
    chainLink->getGraphical().setBaseTexture(*chainLinkTex);
    Matrix4& adjust = chainLink->getGraphical().getAdjustmentMatrix();
    tempm.createScaleMatrix(0.001, 0.001, 0.001);
    adjust.multiply( tempm );
    tempm.createRotationMatrix(90.0f*M_PI/180.0f, 1.0f, 0.0f, 0.0f);
    adjust.multiply( tempm );
    objects.push_back(chainLink);
	
        
    // set eye level
    cameraFrame.getLocation().set(0.0f, 6 * FOOT, ROOM_SIZE);
	cameraFrame.getForwardVector().set(0.0f, 0.0f, -1.0f);
	
	// init renderer
	renderer = new HemRenderer(resourceManager);
	renderer->setCamera( &cameraFrame );
	renderer->setLightSource( lightPos );
	renderer->setSkyColor( skyColor );
	renderer->setGroundColor( groundColor );
	
	// init 2D renderer
	renderer2D = new Renderer2D(resourceManager);
	
	srand(time(NULL));
}


bool paused = false;
/** Called to render each and every frame
 */
void renderScene(void)
{
    
	// perform bullet physics
	if (!paused && physicsTimer.getElapsedTime() > (1/60.0f) )
	{
	    float t = physicsTimer.getElapsedTime();
	    physicsTimer.reset();
	    dynamicsWorld->stepSimulation(t,10);
	}
	
	static float lastTime = 0.0f;
	lastTime = timer.getElapsedTime();
	
	// Clear the color and depth buffers
	graphics.clearDisplay();
	
	// enable blending so transparency can happen
	graphics.enableBlending();
	
	// object render loop
	renderer->render( objects );
	
	// 2d object render loop
	renderer2D->render( objects2d );
	
	// draw decals
	/*it = decals.begin();
    std::map<Object*,Object*>::iterator rit;
    for (; it != decals.end(); it++)
	{
        Matrix4 positionMatrix;
        Matrix4 transformMatrix;
        
        // get related object if one
        rit = relations.find( (*it) );
        if ( rit == relations.end())
        {
            (*it)->getPosition().getTransformMatrix(positionMatrix);
        }
        else
        {
            Position p(rit->second->getPosition());
            p.translateLocal( 0.0f, 2.0f, 0.0f );
            p.getTransformMatrix(positionMatrix);
        }
        transformMatrix.multiply(cameraMatrix, positionMatrix);
        
        // pick and prep shader
        Matrix4 mvp;
        Matrix3 normal;
        mvp.multiply(projectionMatrix, transformMatrix);
        transformMatrix.extractRotation(normal);
        
        shader = (*it)->getGraphical().getShader();
        shader->use();
        shader->setUniformMatrix( "mvMatrix",         4, transformMatrix.getArray()          );
        shader->setUniformMatrix( "mvpMatrix",        4, mvp.getArray()                      );
        shader->setUniformMatrix( "normalMatrix",     3, normal.getArray()                   );
        shader->setUniformf(      "lightPosition", lightPos.getX(), lightPos.getY(), lightPos.getZ() );
        shader->setUniformfv(     "skyColor",         3, skyColor.getInternal()     );
        shader->setUniformfv(     "groundColor",      3, groundColor.getInternal()           );
        shader->setTexture(       "textureMap",          (*it)->getGraphical().getBaseTexture()             );
       
        // draw object, make sure to lie to the depth buffer :) 
        graphics.setDepthOffset(-1.0f);
        (*it)->getGraphical().draw();
        graphics.disableDepthOffset();
    }*/

	
    // Do the buffer Swap
    graphics.swapBuffers();
    
    // calculate fps
    static float lastsec = 0.0f;
    static int frames = 0;
    if ( (timer.getElapsedTime() - lastsec) >= 1.0f)
    {
        cout << "fps: " << frames << endl;
        frames = 0;
        lastsec = timer.getElapsedTime();
    }
    frames++;
    
#ifdef UNLIMITED_FPS
    //glutPostRedisplay();
#endif
}

/** Called when a normal key is pressed on the keyboard
 * @param key the key pressed
 * @param x the x-coord of the mouse at the time of the press
 * @param y the y-coord of the mouse at the time of the press
 */
void keyPressed(SDL_keysym* key)
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
	
	
	
	switch(key->sym)
	{
		// space
		case ' ':
			/*if (wireframe)
				wireframe = false;
			else
				wireframe = true;*/
			cameraFrame.getLocation().set(0.0f, 6.0f * FOOT, 20.0f * FOOT);
			cameraFrame.getForwardVector().set(0.0f, 0.0f, -1.0f);
			cameraFrame.getUpVector().set(0.0f, 1.0f, 0.0f);
			
			// manually set new position for ball
			btBall->getPosition().getLocation().setY(10*FOOT);
			btBall->syncPositionToPhysics();
			break;
			
		// escape
		case 0x1B:
			exit(1);
			
		// w, forward
		case 'w':
			cameraFrame.translate(cameraFrame.getForwardVector().getX()*FOOT, 0.0f, 
								  cameraFrame.getForwardVector().getZ()*FOOT);
			break;
			
		// s, backward
		case 's':
			cameraFrame.translate(-cameraFrame.getForwardVector().getX()*FOOT, 0.0f, 
								  -cameraFrame.getForwardVector().getZ()*FOOT);
			break;
			
		// a, strafe left
		case 'a':
			cameraFrame.getLocalXAxis(side);
			cameraFrame.translate(side.getX()*FOOT, 0.0f, side.getZ()*FOOT);
			break;
			
		// d, strafe right
		case 'd':
			// can only move in the xz plane
			cameraFrame.getLocalXAxis(side);
			cameraFrame.translate(-side.getX()*FOOT, 0.0f, -side.getZ()*FOOT);
			break;
			
		case '-':
			cameraFrame.translate(0.0f, -1.0f, 0.0f);
			break;
			
		case '=':
			cameraFrame.translate(0.0f, 1.0f, 0.0f);
			break;
			
		case 'g':
			t = new Object(*bigSphere, 300, Point3(0.0f, 0.0f, 0.0f)); // 1 kg sphere
			t->getGraphical().setBaseTexture(*marbleTex);
			objects.push_back(t);
			dynamicsWorld->addRigidBody(t->getPhysical().getRigidBody());
            
            // add decal
            decal = new Object( *decalSurface, 0, Point3( 0.0f, 0.0f, 0.0f ) );
            decal->getGraphical().setBaseTexture( *circleTex );
            decals.push_back(decal);
            relations.insert( std::pair<Object*,Object*>( decal, t ) );
			break;
		case 'p':
			if (paused)
				paused = false;
			else
				paused = true;
			break;
			
		case 'z':
			it = objects.begin() + 2;
			for (; it != objects.end(); it++)
			{
				dynamicsWorld->removeRigidBody((*it)->getPhysical().getRigidBody());
				delete (*it);
			}
			objects.erase(objects.begin()+2, objects.end());
            decals.clear();
            relations.clear();
			break;

        case SDLK_UNKNOWN:
        case SDLK_BACKSPACE:
        case SDLK_TAB:
        case SDLK_CLEAR:
        case SDLK_RETURN:
        case SDLK_PAUSE:
        case SDLK_EXCLAIM:
        case SDLK_QUOTEDBL:
        case SDLK_HASH:
        case SDLK_DOLLAR:
        case SDLK_AMPERSAND:
        case SDLK_QUOTE:
        case SDLK_LEFTPAREN:
        case SDLK_RIGHTPAREN:
        case SDLK_ASTERISK:
        case SDLK_PLUS:
        case SDLK_COMMA:
        case SDLK_PERIOD:
        case SDLK_SLASH:
        case SDLK_0:
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_5:
        case SDLK_6:
        case SDLK_7:
        case SDLK_8:
        case SDLK_9:
        case SDLK_COLON:
        case SDLK_SEMICOLON:
        case SDLK_LESS:
        case SDLK_GREATER:
        case SDLK_QUESTION:
        case SDLK_AT:
        case SDLK_LEFTBRACKET:
        case SDLK_BACKSLASH:
        case SDLK_RIGHTBRACKET:
        case SDLK_CARET:
        case SDLK_UNDERSCORE:
        case SDLK_BACKQUOTE:
        case SDLK_b:
        case SDLK_c:
        case SDLK_e:
        case SDLK_f:
        case SDLK_h:
        case SDLK_i:
        case SDLK_j:
        case SDLK_k:
        case SDLK_l:
        case SDLK_m:
        case SDLK_n:
        case SDLK_o:
        case SDLK_q:
        case SDLK_r:
        case SDLK_t:
        case SDLK_u:
        case SDLK_v:
        case SDLK_x:
        case SDLK_y:
        case SDLK_DELETE:
        case SDLK_WORLD_0:
        case SDLK_WORLD_1:
        case SDLK_WORLD_2:
        case SDLK_WORLD_3:
        case SDLK_WORLD_4:
        case SDLK_WORLD_5:
        case SDLK_WORLD_6:
        case SDLK_WORLD_7:
        case SDLK_WORLD_8:
        case SDLK_WORLD_9:
        case SDLK_WORLD_10:
        case SDLK_WORLD_11:
        case SDLK_WORLD_12:
        case SDLK_WORLD_13:
        case SDLK_WORLD_14:
        case SDLK_WORLD_15:
        case SDLK_WORLD_16:
        case SDLK_WORLD_17:
        case SDLK_WORLD_18:
        case SDLK_WORLD_19:
        case SDLK_WORLD_20:
        case SDLK_WORLD_21:
        case SDLK_WORLD_22:
        case SDLK_WORLD_23:
        case SDLK_WORLD_24:
        case SDLK_WORLD_25:
        case SDLK_WORLD_26:
        case SDLK_WORLD_27:
        case SDLK_WORLD_28:
        case SDLK_WORLD_29:
        case SDLK_WORLD_30:
        case SDLK_WORLD_31:
        case SDLK_WORLD_32:
        case SDLK_WORLD_33:
        case SDLK_WORLD_34:
        case SDLK_WORLD_35:
        case SDLK_WORLD_36:
        case SDLK_WORLD_37:
        case SDLK_WORLD_38:
        case SDLK_WORLD_39:
        case SDLK_WORLD_40:
        case SDLK_WORLD_41:
        case SDLK_WORLD_42:
        case SDLK_WORLD_43:
        case SDLK_WORLD_44:
        case SDLK_WORLD_45:
        case SDLK_WORLD_46:
        case SDLK_WORLD_47:
        case SDLK_WORLD_48:
        case SDLK_WORLD_49:
        case SDLK_WORLD_50:
        case SDLK_WORLD_51:
        case SDLK_WORLD_52:
        case SDLK_WORLD_53:
        case SDLK_WORLD_54:
        case SDLK_WORLD_55:
        case SDLK_WORLD_56:
        case SDLK_WORLD_57:
        case SDLK_WORLD_58:
        case SDLK_WORLD_59:
        case SDLK_WORLD_60:
        case SDLK_WORLD_61:
        case SDLK_WORLD_62:
        case SDLK_WORLD_63:
        case SDLK_WORLD_64:
        case SDLK_WORLD_65:
        case SDLK_WORLD_66:
        case SDLK_WORLD_67:
        case SDLK_WORLD_68:
        case SDLK_WORLD_69:
        case SDLK_WORLD_70:
        case SDLK_WORLD_71:
        case SDLK_WORLD_72:
        case SDLK_WORLD_73:
        case SDLK_WORLD_74:
        case SDLK_WORLD_75:
        case SDLK_WORLD_76:
        case SDLK_WORLD_77:
        case SDLK_WORLD_78:
        case SDLK_WORLD_79:
        case SDLK_WORLD_80:
        case SDLK_WORLD_81:
        case SDLK_WORLD_82:
        case SDLK_WORLD_83:
        case SDLK_WORLD_84:
        case SDLK_WORLD_85:
        case SDLK_WORLD_86:
        case SDLK_WORLD_87:
        case SDLK_WORLD_88:
        case SDLK_WORLD_89:
        case SDLK_WORLD_90:
        case SDLK_WORLD_91:
        case SDLK_WORLD_92:
        case SDLK_WORLD_93:
        case SDLK_WORLD_94:
        case SDLK_WORLD_95:
        case SDLK_KP0:
        case SDLK_KP1:
        case SDLK_KP2:
        case SDLK_KP3:
        case SDLK_KP4:
        case SDLK_KP5:
        case SDLK_KP6:
        case SDLK_KP7:
        case SDLK_KP8:
        case SDLK_KP9:
        case SDLK_KP_PERIOD:
        case SDLK_KP_DIVIDE:
        case SDLK_KP_MULTIPLY:
        case SDLK_KP_MINUS:
        case SDLK_KP_PLUS:
        case SDLK_KP_ENTER:
        case SDLK_KP_EQUALS:
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_RIGHT:
        case SDLK_LEFT:
        case SDLK_INSERT:
        case SDLK_HOME:
        case SDLK_END:
        case SDLK_PAGEUP:
        case SDLK_PAGEDOWN:
        case SDLK_F1:
        case SDLK_F2:
        case SDLK_F3:
        case SDLK_F4:
        case SDLK_F5:
        case SDLK_F6:
        case SDLK_F7:
        case SDLK_F8:
        case SDLK_F9:
        case SDLK_F10:
        case SDLK_F11:
        case SDLK_F12:
        case SDLK_F13:
        case SDLK_F14:
        case SDLK_F15:
        case SDLK_NUMLOCK:
        case SDLK_CAPSLOCK:
        case SDLK_SCROLLOCK:
        case SDLK_RSHIFT:
        case SDLK_LSHIFT:
        case SDLK_RCTRL:
        case SDLK_LCTRL:
        case SDLK_RALT:
        case SDLK_LALT:
        case SDLK_RMETA:
        case SDLK_LMETA:
        case SDLK_LSUPER:
        case SDLK_RSUPER:
        case SDLK_MODE:
        case SDLK_COMPOSE:
        case SDLK_HELP:
        case SDLK_PRINT:
        case SDLK_SYSREQ:
        case SDLK_BREAK:
        case SDLK_MENU:
        case SDLK_POWER:
        case SDLK_EURO:
        case SDLK_UNDO:
        case SDLK_LAST:
        default:
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
void mouseClicked(int button, int state, int x, int y)
{
	if (state == SDL_RELEASED)
		return;
	
	
	Position p;
	Object* t;
	static float speed = 4700 * 300;
	
	switch(button)
	{
		case SDL_BUTTON_LEFT:
			p.set(cameraFrame);
			p.translateLocal(0.0f, -1.5*FOOT, -2.0*FOOT);
			
			t = new Object(*sphere, 500, p); // 1 kg sphere
			t->getGraphical().setBaseTexture(*marbleTex);
			objects.push_back(t);
			t->getPhysical().getRigidBody()->applyForce(btVector3(p.getForwardVector().getX()*speed, 
										p.getForwardVector().getY()*speed, p.getForwardVector().getZ()*speed), 
										  btVector3(0.0f, 0.0f, 0.0f));
			dynamicsWorld->addRigidBody(t->getPhysical().getRigidBody());
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
	// avoid reprocess from warp pointer call
	if (x == (screenWidth/2) && y == (screenHeight/2))
		return;
	
	// rotate on Y-axis (around X-axis)
	// rotate with rederence to local coords
	int m = 0;
	if (y > (screenHeight/2))
		m = (y - (screenHeight/2));
	else
		m = -((screenHeight/2) - y);
	float yDeg = m * Y_AXIS_SENSITIVITY;
	cameraFrame.rotateLocal(yDeg / 180.0f * M_PI, Vector3(1.0f, 0.0f, 0.0f));

	
	// check Y-axis bounds
	Vector3& forward = cameraFrame.getForwardVector();
	Vector3& up = cameraFrame.getUpVector();
	// enforce bounds by never allowing the up (Y-axis) vector to go negative
	while (up.getY() < 0)
	{
		if (forward.getY() > 0)
			cameraFrame.rotateLocal(1.0f / 180.0f * M_PI, Vector3(1.0f, 0.0f, 0.0f));
		else
			cameraFrame.rotateLocal(-1.0f / 180.0f * M_PI, Vector3(1.0f, 0.0f, 0.0f));
	}
	
	// rotate on X-axis (around Y-axis)
	// rotate with reference to world coords
	if (x > (screenWidth/2))
		m = -(x - (screenWidth/2));
	else
		m = ((screenWidth/2) - x);
	float xDeg = m * X_AXIS_SENSITIVITY;
	cameraFrame.rotate(xDeg  * (M_PI / 180.0f), Vector3(0.0f, 1.0f, 0.0f));

	graphics.warpMouse(screenWidth / 2, screenHeight / 2);
}


/** Main program entry point
 */
int main(int argc, char* argv[])
{
	
	graphics.init();
	graphics.setDisplaySize( 640, 480 );
	graphics.createScreen();
	
	// perform setup
	setup();
	SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL);
	graphics.showCursor( false );
	changeWindowSize(640,480); // to ensure transform pipeline is setup
	
	// run SDL
	SDL_Event event;
	while( true )
	{
	    while ( SDL_PollEvent( &event ) )
	    {
	        switch( event.type )
	        {
	            case SDL_QUIT:
	                exit(0);
	                
	            case SDL_VIDEORESIZE:
	                graphics.setDisplaySize( event.resize.w, event.resize.h );
	                graphics.createScreen();
	                changeWindowSize( event.resize.w, event.resize.h );
	                break;
	                
	            case SDL_KEYDOWN:
	                keyPressed( &event.key.keysym );
	                break;
	                
	            case SDL_MOUSEMOTION:
	                mouseMovedPassive( event.motion.x, event.motion.y );
	                break;
	                
	            case SDL_MOUSEBUTTONDOWN:
	                mouseClicked( event.button.button, event.button.state, 
	                    event.button.x, event.button.y );
	                break;
	                
	        }
	    }
	        
	    renderScene();
	}

	// enter (and never exit) main execution loop
	//glutMainLoop();
	
	
    
	return 0;
}


































