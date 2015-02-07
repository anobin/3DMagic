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

#include <algorithm>

#include <World/World.h>
#include <Cameras/FPCamera.h>
#include <Shaders\GpuProgram.h>

namespace Magic3D
{
    

void World::stepPhysics()
{
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
}
    
    
void World::renderObjects()
{   
	StopWatch timer;

    // ensure that we have a camera
    MAGIC_THROW(camera == NULL, "Tried to process a frame without a camera set." );
    
    // Clear the color and depth buffers
	graphics.clearDisplay();
	
	// get view and projection matrices for scene (same for all objects)
    Matrix4 view;
    camera->getPosition().getCameraMatrix(view);
    const Matrix4& projection = camera->getProjectionMatrix();
    
    // setup wireframe if set to
    if (wireframeEnabled)
    {
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }


	std::vector<Object*> sortedObjects;
	sortedObjects.reserve(this->objects.size());
	sortedObjects.insert(sortedObjects.begin(), this->objects.begin(), this->objects.end());

	Point3 loc = camera->getPosition().getLocation();
	std::sort(sortedObjects.begin(), sortedObjects.end(), [&](Object* a, Object* b) -> bool {
		auto aTrans = a->getModel()->getMaterial()->transparent;
		auto bTrans = b->getModel()->getMaterial()->transparent;

		// sort transparent objects to back
		if (!aTrans && bTrans)
			return true; // a should go before as it's not transparent
		else if (aTrans && !bTrans)
			return false; // b should go before as it's not transparent

		// sort opaque objects from front to back, to take advantage of depth buffer
		if (!aTrans)
		{
			return loc.distanceTo(a->getPosition().getLocation()) <
				loc.distanceTo(b->getPosition().getLocation());
		}
		// sort transparent objects from back to front, to ensure rendering works
		else
		{
			return loc.distanceTo(a->getPosition().getLocation()) >
				loc.distanceTo(b->getPosition().getLocation());
		}
	});

	// start off with depth buffer writes on
	glDepthMask(GL_TRUE);

	// render all objects
	std::vector<Object*>::iterator it = sortedObjects.begin();
	Object* ob;
	std::shared_ptr<GpuProgram> gpuProgram; 
	vertexCount = 0;
	for(; it != sortedObjects.end(); it++)
	{
	    // get object and entity
	    ob = (*it);
	    
		const std::shared_ptr<Meshes> meshes = ob->getModel()->getMeshes();
		if (meshes == nullptr)
			break;

        // get mesh and material data
		auto material = ob->getModel()->getMaterial();

		// disable depth buffer writes once we get to the transparent objects
		if (material->transparent)
			glDepthMask(GL_FALSE);
            
        // get model/world matrix for object (same for all meshes in object)
        Matrix4 model;
        ob->getPosition().getTransformMatrix(model);
                
        // ensure there is a gpuProgram
        gpuProgram = material->gpuProgram;
        MAGIC_ASSERT(gpuProgram != NULL);

		for(const std::shared_ptr<Mesh> mesh : *meshes)
		{   
			// 'use' gpuProgram
			gpuProgram->use();
                
			// set named uniforms
			for(unsigned int i=0; i < gpuProgram->namedUniforms.size(); i++)
			{
				GpuProgram::NamedUniform& u = *gpuProgram->namedUniforms[i];
				switch(u.datatype)
				{
					case VertexArray::FLOAT:
						gpuProgram->setUniformfv(u.varName.c_str(), u.comp_count, (const float*)u.data);
						break;
					case VertexArray::INT:
						gpuProgram->setUniformiv(u.varName.c_str(), u.comp_count, (const int*)u.data);
						break;
					default:
						throw_MagicException("Unsupported Auto Uniform datatype." );
						break;
				}
			}
                
			// set auto uniforms
			Matrix4 temp4m;
			Matrix4 temp4m2;
			Matrix3 temp3m;
			Point3 tempp3;
			for(unsigned int i=0; i < gpuProgram->autoUniforms.size(); i++)
			{
				GpuProgram::AutoUniform& u = *gpuProgram->autoUniforms[i];
				switch (u.type)
				{
					case GpuProgram::MODEL_MATRIX:                   // mat4
						gpuProgram->setUniformMatrix( u.varName.c_str(), 4, model.getArray() );
						break;
					case GpuProgram::VIEW_MATRIX:                    // mat4
						gpuProgram->setUniformMatrix( u.varName.c_str(), 4, view.getArray() );
						break;
					case GpuProgram::PROJECTION_MATRIX:              // mat4
						gpuProgram->setUniformMatrix( u.varName.c_str(), 4, projection.getArray() );
						break;
					case GpuProgram::MODEL_VIEW_MATRIX:              // mat4
						temp4m.multiply(view, model);
						gpuProgram->setUniformMatrix( u.varName.c_str(), 4, temp4m.getArray() );
						break;
					case GpuProgram::VIEW_PROJECTION_MATRIX:         // mat4
						temp4m.multiply(projection, view);
						gpuProgram->setUniformMatrix(u.varName.c_str(), 4, temp4m.getArray());
						break;
					case GpuProgram::MODEL_PROJECTION_MATRIX:        // mat4
						temp4m.multiply(projection, model);
						gpuProgram->setUniformMatrix(u.varName.c_str(), 4, temp4m.getArray());
						break;
					case GpuProgram::MODEL_VIEW_PROJECTION_MATRIX:   // mat4
						temp4m.multiply(view, model);
						temp4m2.multiply(projection, temp4m);
						gpuProgram->setUniformMatrix(u.varName.c_str(), 4, temp4m2.getArray());
						break;
					case GpuProgram::NORMAL_MATRIX:                  // mat3
						temp4m.multiply(view, model);
						temp4m.extractRotation(temp3m);
						gpuProgram->setUniformMatrix(u.varName.c_str(), 3, temp3m.getArray());
						break;
					case GpuProgram::FPS:                            // int
						gpuProgram->setUniformiv(u.varName.c_str(), 1, &this->actualFPS);
						break;
					case GpuProgram::TEXTURE0:                       // sampler2D
						MAGIC_THROW(material->textures[0] == NULL, "Material has auto-bound "
							"texture set, but no texture set for the index." );
						gpuProgram->setTexture(u.varName.c_str(), material->textures[0].get());
						break;
					case GpuProgram::TEXTURE1:                       // sampler2D
						MAGIC_THROW(material->textures[1] == NULL, "Material has auto-bound "
							"texture set, but no texture set for the index." );
						gpuProgram->setTexture(u.varName.c_str(), material->textures[1].get());
						break;
					case GpuProgram::TEXTURE2:                       // sampler2D
						MAGIC_THROW(material->textures[2] == NULL, "Material has auto-bound "
							"texture set, but no texture set for the index." );
						gpuProgram->setTexture(u.varName.c_str(), material->textures[2].get());
						break;
					case GpuProgram::TEXTURE3:                       // sampler2D
						MAGIC_THROW(material->textures[3] == NULL, "Material has auto-bound "
							"texture set, but no texture set for the index." );
						gpuProgram->setTexture(u.varName.c_str(), material->textures[3].get());
						break;
					case GpuProgram::TEXTURE4:                       // sampler2D
						MAGIC_THROW(material->textures[4] == NULL, "Material has auto-bound "
							"texture set, but no texture set for the index." );
						gpuProgram->setTexture(u.varName.c_str(), material->textures[4].get());
						break;
					case GpuProgram::TEXTURE5:                       // sampler2D
						MAGIC_THROW(material->textures[5] == NULL, "Material has auto-bound "
							"texture set, but no texture set for the index." );
						gpuProgram->setTexture(u.varName.c_str(), material->textures[5].get());
						break;
					case GpuProgram::TEXTURE6:                       // sampler2D
						MAGIC_THROW(material->textures[6] == NULL, "Material has auto-bound "
							"texture set, but no texture set for the index." );
						gpuProgram->setTexture(u.varName.c_str(), material->textures[6].get());
						break;
					case GpuProgram::TEXTURE7:                       // sampler2D
						MAGIC_THROW(material->textures[7] == NULL, "Material has auto-bound "
							"texture set, but no texture set for the index." );
						gpuProgram->setTexture(u.varName.c_str(), material->textures[7].get());
						break;
					case GpuProgram::LIGHT_LOCATION:                 // vec3
						MAGIC_THROW(light == NULL, "Material has the light location "
							"auto-bound uniform set, but no light is set for the world." );
						tempp3 = light->getLocation().transform(view);
						gpuProgram->setUniformf(u.varName.c_str(), tempp3.x(),
							tempp3.y(), tempp3.z() );
						break;
					case GpuProgram::FLAT_PROJECTION:   // mat4
						temp4m.createOrthographicMatrix(0, (Scalar)this->graphics.getDisplayWidth(), 
							0, (Scalar)this->graphics.getDisplayHeight(), -1.0, 1.0);
						gpuProgram->setUniformMatrix(u.varName.c_str(), 4, temp4m.getArray());
						break;
					default:
						MAGIC_ASSERT( false );
						break;
				}
			}
                
			// check for a depth lie
			if (material->depthBufferLie)
			{
				glPolygonOffset( material->depthBufferLie, 1.0f );
				glEnable(GL_POLYGON_OFFSET_FILL);
			}
                
			// draw mesh
			mesh->getVertexArray().draw(mesh->getPrimitive(), mesh->getVertexCount());
			vertexCount += mesh->getVertexCount();
                
			// disable depth lie if it was enabled
			if (material->depthBufferLie)
				glDisable(GL_POLYGON_OFFSET_FILL);
		}
	    
	} // end of all objects 
	
	// restore after wireframe
    if (wireframeEnabled)
    {
        glDisable(GL_LINE_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
	
	// Do the buffer Swap
    graphics.swapBuffers();

	// re-enable depth buffer writes at the end so that depth buffer can be cleared for the
	// next frame correctly
	glDepthMask(GL_TRUE);

	this->renderTimeElapsed = timer.getElapsedTime();
}










};


















