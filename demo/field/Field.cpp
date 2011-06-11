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

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <list>
using std::cout;
using std::endl;

// include GLUT
#include <GL/glut.h>           

#define FPS2MS(x) (1000 / (x))
#define RGB_SINGLE(x) ((1.0f/255.0f)*(x))
#define RGB(r, g, b) {RGB_SINGLE((r)), RGB_SINGLE((g)), RGB_SINGLE((b)), 1.0f}


#define ROOM_SIZE (20.0f * FOOT)


Matrix4 projectionMatrix;

// resource manager
ResourceManager resourceManager("../../");

// shaders
PointLightDiffuseShader* shader;
FlatShader* flatShader;
SimpleADSShader* adsShader;
HemisphereShader* hemShader;
HemisphereTexShader* hemTexShader;


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

// 3ds test
CustomModel* chainLinkModel;
Texture* chainLinkTex;

// GUI stuff
Rectangle2D* frameModel;
Circle2D* circleModel;
Shader2D* frameShader;
Texture* frameTex;
Texture* circleTex;
Matrix4 flatProjectionMatrix;

// legacy
Position             cameraFrame;

bool wireframe = false;

int screenWidth = 0;
int screenHeight = 0;

// tracks game time
StopWatch	timer;

/** Called when the window size changes
 * @param w width of the new window
 * @param h hieght of the new window
 */
void changeWindowSize(int w, int h)
{
	glViewport(0, 0, w, h);
	
	screenWidth = w;
	screenHeight = h;
	
    // Create the projection matrix
    projectionMatrix.createPerspectiveMatrix(60.0f, float(w)/float(h), INCH, 1000*FOOT);
	
	// create the orthographic/flat project matrix for the screen size and -1 to 1 depth
	flatProjectionMatrix.createOrthographicMatrix(0, w, 0, h, -1.0, 1.0);
	
	frameModel = new Rectangle2D(w-173-10, 10, 173, 50);
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
	
	
	glEnable(GL_DEPTH_TEST);
	
    static Color lightBlue(5, 230, 255, Color::RGBb);
	const GLfloat* c = lightBlue.getInternal();
	glClearColor(c[0], c[1], c[2], 1.0f);
	
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
	if (resourceManager.doesResourceExist("images/marble.tga"))
	{
		Handle<TGA2DResource> marbleImage = resourceManager.get<TGA2DResource>("images/marble.tga");
		marbleTex= new Texture(marbleImage());
		marbleTex->setWrapMode(Texture::CLAMP_TO_EDGE);
	}
	else
	{
		Handle<SingleColor2DResource> marbleImage = resourceManager.injectSingleColor2D(
				"images/marble.tga", Color::GREEN);
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
					"images/singleBrick.tga", Color(255, 118, 27, 255, Color::RGBAb));
		brickTex = new Texture(brickImage());
		brickTex->setWrapMode(Texture::CLAMP_TO_EDGE);
	}
	
	
	// init shaders
	shader = new PointLightDiffuseShader(resourceManager);
	flatShader = new FlatShader(resourceManager);
	adsShader = new SimpleADSShader(resourceManager);
	hemShader = new HemisphereShader(resourceManager);
	hemTexShader = new HemisphereTexShader(resourceManager);
	
	
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
	btBall->setBaseTexture(*marbleTex);
	objects.push_back(btBall);
	//dynamicsWorld->addRigidBody(btBall->getRigidBody());
	
	floorObject = new Object(*floorModel, 0); // static object
	floorObject->setBaseTexture(*stoneTex);
	objects.push_back(floorObject);
	dynamicsWorld->addRigidBody(floorObject->getRigidBody());
	
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
			btBox = new Object(*boxModel, 4, Point3(w, h, zOffset), 3.0f); // 3 kg box
			btBox->setBaseTexture(*brickTex);
			objects.push_back(btBox);
			dynamicsWorld->addRigidBody(btBox->getRigidBody());
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
	frameShader = new Shader2D(resourceManager);
	Handle<SingleColor2DResource> frameImage = resourceManager.injectSingleColor2D(
					"images/frameImage", Color(255, 118, 27, 100, Color::RGBAb));
	if (resourceManager.doesResourceExist("images/logo.tga"))
	{
		Handle<TGA2DResource> logoImage = resourceManager.get
			<TGA2DResource>("images/logo.tga");
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
	
	// 3ds test stuff
	Handle<Model3DSResource> linkResource = 
		resourceManager.get<Model3DSResource>("models/chainLink.3ds");
	chainLinkModel = new CustomModel(*linkResource());
	Handle<SingleColor2DResource> linkImage = resourceManager.injectSingleColor2D(
					"images/linkImage", Color(195, 195, 195, 255, Color::RGBAb));
	chainLinkTex = new Texture(linkImage());
	
	
        
    // set eye level
    cameraFrame.getLocation().set(0.0f, 6 * FOOT, ROOM_SIZE);
	cameraFrame.getForwardVector().set(0.0f, 0.0f, -1.0f);
	
	
	srand(time(NULL));
}


bool paused = false;
/** Called to render each and every frame
 */
void renderScene(void)
{
	// perform bullet physics
	if (!paused)
	  dynamicsWorld->stepSimulation(1/60.f,10);
	
    // Time Based animation
	static float dir = -1.0;
	static float lastTime = 0.0f;
	static float laserDistance = 0.0f;
	laserDistance += (timer.getElapsedTime() - lastTime) * (54*FOOT) * dir;
	
	lastTime = timer.getElapsedTime();
	//bigBall.getPosition().rotate(m3dDegToRad(360.0f/120), Vector3f(0.0f, 1.0f, 0.0f));

	
	// check laser physics
	if (laserDistance < (-ROOM_SIZE))
		dir = 1.0f;
	else if (laserDistance > (ROOM_SIZE))
		dir = -1.0f;
	//laser.getPosition().setLocation(0.0f, 5*FOOT, laserDistance);
	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    
    // Save the current modelview matrix (the identity matrix)
    Matrix4 cameraMatrix;
    cameraFrame.getCameraMatrix(cameraMatrix);

    // Transform the light position into eye coordinates
	Point3 lightPos(0.0f, 1000.0f, 0.0f);
	
	lightPos.transform(cameraMatrix);
	shader->setLightPosition(lightPos);
	adsShader->setLightPosition(lightPos);
	
	// enable blending so transparency can happen
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// object render loop
	std::vector<Object*>::iterator it = objects.begin();
	for (; it != objects.end(); it++)
	{
		// apply position transform
		Matrix4 positionMatrix;
		(*it)->getPosition().getTransformMatrix(positionMatrix);
		Matrix4 transformMatrix;
		transformMatrix.multiply(cameraMatrix, positionMatrix);
		
		// pick and prep shader
		Matrix4 mvp;
		Matrix3 normal;
		mvp.multiply(projectionMatrix, transformMatrix);
		transformMatrix.extractRotation(normal);
		hemTexShader->setMVMatrix(transformMatrix);
		hemTexShader->setMVPMatrix(mvp);
		hemTexShader->setNormalMatrix(normal);
		hemTexShader->setLightPosition(lightPos);
		hemTexShader->setSkyColor(Color(255, 255, 255));
		hemTexShader->setGroundColor(Color(0, 0, 0));
		hemTexShader->setTexture(*(*it)->getBaseTexture());
		hemTexShader->use();
		
		// draw object 
		(*it)->draw();
		
		// unload position transform
	}
	
	// draw chain link
	Matrix4 positionMatrix;
	Matrix4 scale;
	scale.createScaleMatrix(0.01, 0.01, 0.01);
	Position(0.0f, 3.0f*FOOT /** (1.0f/0.01)*/, 0.0f).getTransformMatrix(positionMatrix);
	Matrix4 transformMatrix;
	transformMatrix.multiply(cameraMatrix);
	transformMatrix.multiply(positionMatrix);
	transformMatrix.multiply(scale);
	scale.createRotationMatrix(90.0f*M_PI/180.0f, 1.0f, 0.0f, 0.0f);
	transformMatrix.multiply(scale);
	
	// pick and prep shader
	Matrix4 mvp;
	Matrix3 normal;
	mvp.multiply(projectionMatrix, transformMatrix);
	transformMatrix.extractRotation(normal);
	hemTexShader->setMVMatrix(transformMatrix);
	hemTexShader->setMVPMatrix(mvp);
	hemTexShader->setNormalMatrix(normal);
	hemTexShader->setLightPosition(lightPos);
	hemTexShader->setSkyColor(Color(255, 255, 255));
	hemTexShader->setGroundColor(Color(0, 0, 0));
	hemTexShader->setTexture(*chainLinkTex);
	hemTexShader->use();
	chainLinkModel->draw();
	
	
	// draw GUI stuff
	frameShader->setMVPMatrix(flatProjectionMatrix);
	frameShader->setTexture(*frameTex);
	frameShader->use();
	frameModel->draw(VertexArray::TRIANGLE_FAN);
	frameShader->setTexture(*circleTex);
	frameShader->use();
	circleModel->draw(VertexArray::TRIANGLE_FAN);

	
    // Do the buffer Swap
    glutSwapBuffers();
    
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
    glutPostRedisplay();
#endif
}

/** Called when a normal key is pressed on the keyboard
 * @param key the key pressed
 * @param x the x-coord of the mouse at the time of the press
 * @param y the y-coord of the mouse at the time of the press
 */
void keyPressed(unsigned char key,int x, int y)
{
	Point3 origin;
	Vector3 forward;
	Vector3 side;
	Vector3 up;
	btTransform transform;
	Position p;
	Object* t;
	
	
	
	switch(key)
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
			t->setBaseTexture(*marbleTex);
			objects.push_back(t);
			dynamicsWorld->addRigidBody(t->getRigidBody());
			break;
		case 'p':
			if (paused)
				paused = false;
			else
				paused = true;
			break;
			
		case 'z':
			std::vector<Object*>::iterator it = objects.begin() + 2;
			for (; it != objects.end(); it++)
			{
				dynamicsWorld->removeRigidBody((*it)->getRigidBody());
				delete (*it);
			}
			objects.erase(objects.begin()+2, objects.end());
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
	if (state == GLUT_UP)
		return;
	
	
	Position p;
	Object* t;
	static float speed = 4700 * 300;
	
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			p.set(cameraFrame);
			p.translateLocal(0.0f, -1.5*FOOT, -2.0*FOOT);
			
			t = new Object(*sphere, 500, p); // 1 kg sphere
			t->setBaseTexture(*marbleTex);
			objects.push_back(t);
			t->getRigidBody()->applyForce(btVector3(p.getForwardVector().getX()*speed, 
										p.getForwardVector().getY()*speed, p.getForwardVector().getZ()*speed), 
										  btVector3(0.0f, 0.0f, 0.0f));
			dynamicsWorld->addRigidBody(t->getRigidBody());
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

	
	glutWarpPointer(screenWidth / 2, screenHeight / 2);
}


/** Called to regulate framerate
 */
void frameRateRegulator(int value)
{
	// post a redisplay 
	glutPostRedisplay();
	
	// reregister callback
#ifndef UNLIMITED_FPS
	glutTimerFunc(FPS2MS(60), frameRateRegulator, 0);
#endif
}


/** Main program entry point
 */
int main(int argc, char* argv[])
{
	
	// init glut with args
	glutInit(&argc, argv);
	// Double-buffered, RGBA display with depth and stencil buffers
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	// initial window size
	glutInitWindowSize(800, 600);
	// create window with title
	glutCreateWindow("The Room");
	
	glutWarpPointer(screenWidth / 2, screenHeight / 2);
	
	// register event funtions
    glutReshapeFunc(changeWindowSize); // window resized
    glutDisplayFunc(renderScene); // to render frame
	glutKeyboardFunc(keyPressed); // normal key pressed
	glutSpecialFunc(specialKeyPressed); // special key pressed
	glutMouseFunc(mouseClicked); // mouse button clicked
	glutMotionFunc(mouseMoved); // mouse clicked and moved
	glutPassiveMotionFunc(mouseMovedPassive); // mouse moved unclicked
	frameRateRegulator(0); // called to regulate frame rate


	// init GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}
	
	// perform setup
	setup();
	changeWindowSize(1,1); // to ensure transform pipeline is setup

	// enter (and never exit) main execution loop
	glutMainLoop();
	
	return 0;
}


































