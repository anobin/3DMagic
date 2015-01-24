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

#include "../DemoBase.h"

// test
#include <Math/Generic/Vector.h>

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
#include <memory>
#include <sstream>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::shared_ptr;

// include freetype
#include <ft2build.h>
#include FT_FREETYPE_H // yes it's a macro include and yes it's the standard way

#define ROOM_SIZE (20.0f * FOOT)


Matrix4 projectionMatrix;

// resource manager
ResourceManager resourceManager("../../../../resources/");

// textures
Texture* stoneTex = NULL;
Texture* marbleTex = NULL;
Texture* bunkerTex = NULL;
Texture* brickTex = NULL;
Texture* blueTex = NULL;
Texture* charTex = NULL;

// batches
Batch floorBatch;
Batch sphereBatch;
Batch tinySphereBatch;
Batch bigSphereBatch;
Batch boxBatch;

// materials
Material floorMaterial;
Material sphereMaterial;
Material tinySphereMaterial;
Material bigSphereMaterial;
Material boxMaterial; 

// collisions shapes
PlaneCollisionShape floorShape( Vector3(0,1,0) );
SphereCollisionShape sphereShape( 2*FOOT );
SphereCollisionShape tinySphereShape( 1*FOOT );
BoxCollisionShape bigSphereShape( 3, 3, 3 );
BoxCollisionShape boxShape( 6*INCH*5.0f, 3*INCH*5.0f, 3*INCH*5.0f );

// objects
Object* bigBall;
Object* laser;
Object* floorObject;
Object* ceiling;
Object* wallObject;

// shader uniforms

Color groundColor(25,25,25);
float groundColorf[3];
Color skyColor(255,255,255);
float skyColorf[3];

// shaders
Shader* shader;
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

// builders
BatchBuilder batchBuilder;
MaterialBuilder materialBuilder;

// 3ds stuff
Mesh* chainMeshes;
Material chainMaterial;
Texture* chainTex;
Object* chainObject;

//FT_Face face;
StaticFont* font;
Image charImage(120, 120, 4);

Object* btBall; // graphical presence of ball used for bullet
Object* btBox;
bool fun = false;


bool paused = true;
int slow = 0;
float change = -1.0f;

/** Called when a normal key is pressed on the keyboard
 * @param key the key pressed
 * @param x the x-coord of the mouse at the time of the press
 * @param y the y-coord of the mouse at the time of the press
 */
void keyPressed(int key, FPCamera& camera, GraphicsSystem& graphics, World& world)
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
    PhysicalBody::Properties prop;
    
	
	
	
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
		    prop.mass = 1;
			t = new Object(std::make_shared<Meshes>(bigSphereBatch), &bigSphereMaterial, &bigSphereShape, prop );
			t->setLocation(Point3(0.0f, 5.0f, 0.0f));
			world.addObject(t);
			
			camera.lookat( Point3(0, 30, 0) );
			
			break;
		case 'h':
		    releaseWater = true;
			break;
		case 'p':
			if (paused)
			{
				paused = false;
				world.alignPhysicsStepToFPS(true);
			}
			else
			{
				paused = true;
				world.alignPhysicsStepToFPS(false);
				world.setPhysicsStepsPerFrame(0);
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
		    
		case 'x':
		    fun = !fun;
		    break;
		    
		case 't':
		    if (chainObject)
		    {
		        world.removeObject(chainObject);
		        delete chainObject;
		        chainObject = NULL;
		    }
		    break;
		    
		case 'k':
		    wireframe = !wireframe;
		    world.setWireFrame(wireframe);

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
void mouseClicked(Event::MouseButtons button, int x, int y, FPCamera& camera, World& world)
{
	
	Position p;
	Object* t;
	static float speed = 1000 * 300;
	PhysicalBody::Properties prop;
	
	switch(button)
	{
	    case Event::LEFT:
			p.set(camera.getPosition());
			p.translateLocal(0.0f, -1.5f*FOOT, -2.0f*FOOT);
			
			prop.mass = 100;
			t = new Object(std::make_shared<Meshes>(sphereBatch), &sphereMaterial, &sphereShape, prop);
			t->setPosition(p);
			world.addObject(t);
			t->getPhysical()->applyForce(Vector3(p.getForwardVector().x()*speed, 
		        p.getForwardVector().y()*speed, p.getForwardVector().z()*speed) );
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
void mouseMovedPassive(int x, int y, FPCamera& camera, GraphicsSystem& graphics)
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

class Sandbox : public DemoBase
{
	Texture* screenTex;

public:

	void setup()
	{

		// bullet setup
		physics.setGravity(0,-9.8f*METER,0);

		graphics.enableDepthTest();

		static Color lightBlue(5, 230, 255);
		graphics.setClearColor(lightBlue );

		// init textures
		if (resourceManager.doesResourceExist("images/bareConcrete.tga"))
		{
			shared_ptr<Image> stoneImage = resourceManager.get<Image>("images/bareConcrete.tga");
			stoneTex = new Texture(*stoneImage);
		}
		else
		{
			Image stoneImage( 1, 1, 4, Color::WHITE);
			stoneTex = new Texture(stoneImage);
		}
		if (resourceManager.doesResourceExist("images/marble.png"))
		{
			shared_ptr<Image> marbleImage = resourceManager.get<Image>("images/marble.png");
			marbleTex= new Texture(*marbleImage);
			marbleTex->setWrapMode(Texture::CLAMP_TO_EDGE);
		}
		else
		{
			Image marbleImage( 1, 1, 4, Color::RED);
			marbleTex= new Texture(marbleImage);
			marbleTex->setWrapMode(Texture::CLAMP_TO_EDGE);
		}
		if (resourceManager.doesResourceExist("images/ConcreteBunkerDirty.tga"))
		{
			shared_ptr<Image> bunkerImage = resourceManager.get<Image>("images/ConcreteBunkerDirty.tga");
			bunkerTex = new Texture(*bunkerImage);
		}
		if (resourceManager.doesResourceExist("images/singleBrick.tga"))
		{
			shared_ptr<Image> brickImage = resourceManager.get<Image>("images/singleBrick.tga");
			brickTex = new Texture(*brickImage);
			brickTex->setWrapMode(Texture::CLAMP_TO_EDGE);
		}
		else
		{
			Image brickImage( 1, 1, 4, Color(255, 118, 27, 255));
			brickTex = new Texture(brickImage);
			brickTex->setWrapMode(Texture::CLAMP_TO_EDGE);
		}
		Image blueImage( 1, 1, 4, Color(31, 97, 240, 255) );
		blueTex = new Texture(blueImage);
		blueTex->setWrapMode(Texture::CLAMP_TO_EDGE);

		shared_ptr<FontResource> dejavuResource = resourceManager.get<FontResource>
			( "fonts/dejavu/DejaVuSerif-Italic.ttf" );
		Character q_char;
		dejavuResource->getMissingChar(&q_char, 20, 20);
		font = new StaticFont(q_char);
		for(unsigned int i=0; i < 128; i++)
		{
			Character* c = new Character();
			dejavuResource->getChar(c, i, 20, 20);
			font->setChar(c);
		}

		charImage.clear(Color::PINK);
		//charImage.copyIn(font->getChar('Q').getBitmap().bitmap);
		charImage.drawAsciiText(*font, "Hola!", 10, 10, Color(255, 0, 0, 255));
		charTex = new Texture(charImage);


		// init shader
		shared_ptr<TextResource> vp = resourceManager.get<TextResource>("shaders/HemisphereTexShader.vp");
		shared_ptr<TextResource> fp = resourceManager.get<TextResource>("shaders/HemisphereTexShader.fp");
		shader = new Shader( vp->getText(), fp->getText() );
		shader->bindAttrib( "vertex" );
		shader->bindAttrib( "normal" );
		shader->bindAttrib( "texcoord0" );
		shader->link();


		// init batches
		batchBuilder.buildSphere(&sphereBatch, 2*FOOT, 55, 32);
		batchBuilder.buildSphere(&tinySphereBatch, 1*FOOT, 4, 4);
		batchBuilder.buildBox(&bigSphereBatch, 3, 3, 3);
		batchBuilder.buildFlatSurface(&floorBatch, ROOM_SIZE*50, ROOM_SIZE*50, 20, 20, 
			true, 15*FOOT, 12*FOOT );
		float scale = 5.0f;
		batchBuilder.buildBox(&boxBatch, 6*INCH*scale, 3*INCH*scale, 3*INCH*scale );

		// init materials
		materialBuilder.begin(&sphereMaterial, 5, 2);
		materialBuilder.setShader(shader);
		materialBuilder.addAutoUniform( "mvMatrix", Material::MODEL_VIEW_MATRIX );
		materialBuilder.addAutoUniform( "mvpMatrix", Material::MODEL_VIEW_PROJECTION_MATRIX );
		materialBuilder.addAutoUniform( "normalMatrix", Material::NORMAL_MATRIX );
		materialBuilder.addAutoUniform( "textureMap", Material::TEXTURE0 );
		materialBuilder.addAutoUniform( "lightPosition", Material::LIGHT_LOCATION);
		skyColor.getColor(skyColorf, 3);
		materialBuilder.addNamedUniform( "skyColor", VertexArray::FLOAT, 3, skyColorf);
		groundColor.getColor(groundColorf, 3);
		materialBuilder.addNamedUniform( "groundColor", VertexArray::FLOAT, 3, groundColorf);
		materialBuilder.setTexture(charTex);
		materialBuilder.end();

		materialBuilder.expand(&tinySphereMaterial, sphereMaterial);
		materialBuilder.setTexture(blueTex);
		materialBuilder.end();

		materialBuilder.expand(&bigSphereMaterial, sphereMaterial);
		materialBuilder.setTexture(charTex);
		materialBuilder.end();

		materialBuilder.expand(&floorMaterial, sphereMaterial);
		materialBuilder.setTexture(stoneTex);
		materialBuilder.end();

		materialBuilder.expand(&boxMaterial, sphereMaterial);
		materialBuilder.setTexture(charTex);
		materialBuilder.end();




		// 2D shader
		shared_ptr<TextResource> vp2D = resourceManager.get<TextResource>("shaders/Shader2D.vp");
		shared_ptr<TextResource> fp2D = resourceManager.get<TextResource>("shaders/Shader2D.fp");
		Shader* shader2D = new Shader(vp2D->getText(), fp2D->getText());
		shader2D->bindAttrib( "vertex" );
		shader2D->bindAttrib( "texcoord0" );
		shader2D->link();

		// circle in middle of screen
		Batch* circle2D = new Batch();
		//batchBuilder.build2DCircle(&circle2D, 0, 0, 50, 5);
		batchBuilder.build2DRectangle(circle2D, 0, 0, 300, 300);

		Image screenImage( 300, 300, 4, Color(31, 97, 240, 255) );
		screenImage.drawAsciiText(*font, "Hola!", 50, 50, Color(255, 255, 255, 255));
		screenTex = new Texture(screenImage);
		screenTex->setWrapMode(Texture::CLAMP_TO_EDGE);

		Material* circle2DMaterial = new Material();
		materialBuilder.begin(circle2DMaterial, 2, 0);
		materialBuilder.setShader(shader2D);
		materialBuilder.addAutoUniform( "mvpMatrix", Material::FLAT_PROJECTION );
		materialBuilder.addAutoUniform( "textureMap", Material::TEXTURE0 );
		materialBuilder.setTexture(screenTex);
		materialBuilder.end();

		world->addObject(new Object(std::make_shared<Meshes>(*circle2D), circle2DMaterial));



		// init objects
		PhysicalBody::Properties prop;
		prop.mass = 1;
		btBall = new Object(std::make_shared<Meshes>(sphereBatch), &sphereMaterial);
		btBall->setLocation(Point3(0.0f, 150*FOOT, 0.0f));
		world->addObject(btBall);

		floorObject = new Object(std::make_shared<Meshes>(floorBatch), &floorMaterial, &floorShape); // static object
		world->addObject(floorObject);

		float wallWidth =40;
		float wallHeight = 10;
		float brickHeight = 3*INCH*scale;
		float brickWidth = 6*INCH*scale;
		float h = brickHeight/2;
		float xOffset = -(brickWidth*wallWidth)/2;
		float zOffset = -100*FOOT;
		prop.friction = 0.8f;
		for (int i=0; i < wallHeight; i++, h+=brickHeight)
		{
			float w = xOffset;
			if (i%2 != 0)
				w = brickWidth/2 + xOffset;
			for (int j=0; j < wallWidth; j++, w+=brickWidth)
			{
				if (i == wallHeight-1 && j == wallWidth-1)
					continue;
				btBox = new Object(std::make_shared<Meshes>(boxBatch), &boxMaterial, &boxShape, prop );
				btBox->setLocation( Point3(w, h, zOffset) );
				world->addObject(btBox);
			}
		}

		// 3ds model
		std::shared_ptr<Meshes> chainBatches = resourceManager.get<Meshes>("models/chainLink.3ds");
		materialBuilder.expand(&chainMaterial, sphereMaterial);
		materialBuilder.setTexture(charTex);
		materialBuilder.end();
		/*modelBuilder.begin(&chainModel, chainResource->getBatchCount());
		for(int i=0; i < chainResource->getBatchCount(); i++)
			modelBuilder.addMesh(&chainMeshes[i], &chainMaterial);
		modelBuilder.end();*/
		//TriangleMeshCollisionShape* chainShape = new TriangleMeshCollisionShape
		//    ( chainBatches, chainResource()->getBatchCount() );
		chainObject = new Object(chainBatches,  &chainMaterial);
		chainObject->setLocation(Point3(0.0f, 5.0f, 0.0f));
		chainObject->getTransformMatrix().createScaleMatrix(0.1, 0.1, 0.1);
		world->addObject(chainObject);



		// set eye level
		camera.setLocation(Point3(0.0f, 6 * FOOT, ROOM_SIZE));
		camera.setStepSpeed( FOOT );
		camera.setStrafeSpeed( FOOT );

		// enable blending so transparency can happen
		graphics.enableBlending();

		srand((unsigned int)time(NULL));
	}


	virtual void tick(void)
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
				PhysicalBody::Properties prop;
				prop.mass = 0.1f;
				Object* t = new Object(std::make_shared<Meshes>(tinySphereBatch), &tinySphereMaterial, &tinySphereShape, prop);
				t->setLocation(Point3(0, 10.0f, 0));
				world->addObject(t);
            
				t->getPhysical()->applyForce(Vector3(((float)(rand()%100))*0.01f, 0.0f, 
					((float)(rand()%100))*0.01f) );
			}
		}
    
		if (lightPos.getLocation().y() <= -400.0f)
			change = 1.0f;
		else if (lightPos.getLocation().y() >= 400.0f)
			change = -1.0f;
		lightPos.setLocation(
			lightPos.getLocation().withY(lightPos.getLocation().y()+change)
		);
    
    
		// fun stuff
		float c1, c2, c3;
		static float acc = 0;
		float c;
		if (fun)
		{
			if ( rand()%2 && acc < 3)
				c = 1.0f;
			else if (acc > -3)
				c = -0.3f;
			else
				c = 0.3f;
			acc += c;
			batchBuilder.modify(&boxBatch);
			for (int i=0; i < boxBatch.getVertexCount(); i++)
			{
				batchBuilder.getVertex3f(&c1, &c2, &c3);
            
				batchBuilder.vertex3f(
					c1<0?c1-c:c1+c, 
					c2<0?c2-c:c2+c, 
					c3<0?c3-c:c3+c
				);
			}
			batchBuilder.end();
		}
    
		char fps[40];
		charImage.clear(Color::BLUE);
		//charImage.copyIn(font->getChar('Q').getBitmap().bitmap);
		sprintf(fps, "Figure: %d", world->getActualFPS() );
		charImage.drawAsciiText(*font, fps, 2, 60, Color::WHITE);
		charTex->set(charImage);

		Point3 endPoint = physics.createRay(camera.getPosition().getLocation(), camera.getPosition().getForwardVector(), 1000);

		btBall->setPosition(Position(
			endPoint,
			btBall->getPosition().getForwardVector(),
			btBall->getPosition().getUpVector()
		));

		std::stringstream ss;
		ss << std::setprecision(2) << std::fixed << endPoint.x() << ", " << endPoint.y() 
			<< ", " << endPoint.z();

		Image screenImage( 300, 300, 4, Color(31, 97, 240, 255) );
		screenImage.drawAsciiText(*font, ss.str().c_str(), 50, 50, Color(255, 255, 255, 255));
		screenTex->set(screenImage);
    
	}

	virtual void handleEvent(const Event& event)
	{
		switch(event.data.type)
		{
			case Event::VIDEO_RESIZE:
				screenHeight = event.data.resize.h;
				screenWidth = event.data.resize.w;
				break;

			case Event::KEY_DOWN:
				keyPressed( event.data.key.key, this->camera, this->graphics, *this->world);
	            break;
	                
	        case Event::MOUSE_MOTION:
				mouseMovedPassive( event.data.motion.x, event.data.motion.y, this->camera, this->graphics );
	            break;
	                
	        case Event::MOUSE_BUTTON_DOWN:
	            mouseClicked( event.data.button.button, event.data.button.x, 
					event.data.button.y, this->camera, *this->world);
	            break;
	                
	        case Event::MOUSE_BUTTON_UP:
	            break;
	                
	        case Event::KEY_UP:
	            keyReleased( event.data.key.key );
	            break;
		}
	}

};


/** Main program entry point
 */
int main(int argc, char* argv[])
{
	
	Sandbox sandbox;

	sandbox.setup();
	sandbox.start();
    
	return 0;
}