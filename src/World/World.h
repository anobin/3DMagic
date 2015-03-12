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
#include <Lights\Light.h>

#include <Resources\ResourceManager.h>

#include <set>
#include <unordered_map>


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

    std::unordered_map<Material*, std::vector<std::shared_ptr<Object>>*> staticObjects;
    int staticObjectCount;
    
    GraphicsSystem& graphics;
    
    PhysicsSystem& physics;
    
    StopWatch frameTimer;
    
    int fps;
    
    float physicsStepTime;
    
    bool alignPStep2FPS;
    
    int physicsStepsPerFrame;
    
    int actualFPS;

	int vertexCount;
    
    Camera* camera;
    
    Light light;
    
    bool wireframeEnabled;

    bool showBoundingSpheres;

    bool showNormals;
    Scalar normalsLength;

    bool useNormalMaps;

    bool useTextures;

    bool castShadows;

    bool showSpecularHighlight;

    bool showCollisionShape;

	float renderTimeElapsed;

    std::shared_ptr<Texture> fallbackTexture;

    std::shared_ptr<GpuProgram> shadowPassProgram;
    std::shared_ptr<Material> shadowPassMaterial;

    GLuint shadowFBO;
    std::shared_ptr<Texture> shadowTex;

    void renderMesh(const TriangleMesh& mesh);

    void setupMaterial(Material& material, const Matrix4& modelMatrix,
        const Matrix4& viewMatrix, const Matrix4& projectionMatrix, bool wireframe,
        Matrix4* shadowMatrix = nullptr, std::shared_ptr<Texture> shadowMap = nullptr);
    void tearDownMaterial(Material& material, bool wireframe);
    
public:
    inline World( GraphicsSystem* graphics, PhysicsSystem* physics, 
        ResourceManager& manager):
        graphics(*graphics), physics(*physics), fps(60), physicsStepTime(1.0f/60.0f),
        alignPStep2FPS(true), physicsStepsPerFrame(1), actualFPS(0), vertexCount(0), camera(NULL),
        wireframeEnabled(false), showBoundingSpheres(false), staticObjectCount(0),
        showNormals(false), useNormalMaps(true), useTextures(true), castShadows(true),
        showSpecularHighlight(true), showCollisionShape(false), normalsLength(1.0f)
    {
        Image fallbackImage(1, 1, 4, Color::WHITE);
        fallbackTexture = std::make_shared<Texture>(fallbackImage);
        fallbackTexture->setWrapMode(Texture::WrapModes::CLAMP_TO_EDGE);

        this->shadowPassProgram = manager.get<GpuProgram>("shaders/Full/ShadowMapPass.gpu.xml");
        this->shadowPassMaterial = std::make_shared<Material>();
        MaterialBuilder b;
        b.begin(this->shadowPassMaterial.get());
        b.setGpuProgram(this->shadowPassProgram);
        b.end();

        shadowTex = std::make_shared<Texture>(GL_DEPTH_COMPONENT32F, 4096, 4096);
        shadowTex->setMinFilter(Texture::MinFilters::LINEAR);
        shadowTex->setMagFilter(Texture::MagFilters::LINEAR);
        shadowTex->setCompareMode(Texture::CompareModes::COMPARE_REF_TO_TEXTURE);
        shadowTex->setCompareFunc(Texture::CompareFuncs::LEQUAL);
        
        glGenFramebuffers(1, &shadowFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, this->shadowFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex->getID(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
	inline void addObject(Object* object)
	{
		objects.insert(object);
		physics.addBody(*object);
	}

    inline void addStaticObject(std::shared_ptr<Object> object)
    {
        auto material = object->getModel()->getMaterial().get();
        auto it = this->staticObjects.find(object->getModel()->getMaterial().get());
        if (it == this->staticObjects.end())
        {
            auto objectList = new std::vector<std::shared_ptr<Object>>();
            objectList->push_back(object);
            this->staticObjects.insert(std::make_pair(material, objectList));
        }
        else
            it->second->push_back(object);
        staticObjectCount++;

        physics.addBody(*object);
    }
   
	inline void removeObject(Object* object)
	{
		std::set<Object*>::iterator it = objects.find(object);
		if ( it != objects.end() )
		{
			objects.erase(it);
			physics.removeBody(*object);
		}
	}
   
	inline void setCamera(Camera* camera)
	{
		this->camera = camera;
	}
   
	inline Light& getLight()
	{
        return light;
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
   
	inline void setWireFrame(bool t)
	{
		this->wireframeEnabled = t;
	}
   
	inline void startFrame()
	{
		frameTimer.reset();
	}
   
	virtual void stepPhysics();
   
	virtual void renderObjects();
   
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

	inline int getVertexCount()
	{
		return vertexCount;
	}

	inline int getObjectCount()
	{
        return this->objects.size() + staticObjectCount;
	}
    
	inline float getRenderTimeElapsed()
	{
		return this->renderTimeElapsed;
	}

    inline void setShowBoundingSpheres(bool show)
    {
        this->showBoundingSpheres = show;
    }

    inline bool getShowBoundingSpheres()
    {
        return this->showBoundingSpheres;
    }

    inline void setShowNormals(bool show)
    {
        this->showNormals = show;
    }

    inline bool isShowNormals()
    {
        return this->showNormals;
    }

    inline void setUseNormalMaps(bool use)
    {
        this->useNormalMaps = use;
    }
    inline bool isUseNormalMaps()
    {
        return this->useNormalMaps;
    }

    inline void setUseTextures(bool use)
    {
        this->useTextures = use;
    }
    inline bool isUseTextures()
    {
        return this->useTextures;
    }

    inline void setCastShadows(bool cast)
    {
        this->castShadows = cast;
    }
    inline bool isCastShadows()
    {
        return this->castShadows;
    }

    inline bool getShowSpecularHighlight()
    {
        return this->showSpecularHighlight;
    }
    inline void setShowSpecularHighlight(bool show)
    {
        this->showSpecularHighlight = show;
    }

    inline bool getShowCollisionShape()
    {
        return this->showCollisionShape;
    }
    inline void setShowCollisionShape(bool show)
    {
        this->showCollisionShape = show;
    }

    inline void setNormalsLength(Scalar length)
    {
        this->normalsLength = length;
    }

    inline Camera& getCamera()
    {
        return *this->camera;
    }
};

};











#endif
