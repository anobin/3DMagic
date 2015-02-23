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
  

void World::setupMaterial(Material& material, const Matrix4& modelMatrix,
    const Matrix4& viewMatrix, const Matrix4& projectionMatrix, bool wireframe)
{
    auto gpuProgram = material.gpuProgram;
    MAGIC_ASSERT(gpuProgram != nullptr);

    // 'use' gpuProgram
    gpuProgram->use();

    // set named uniforms
    for (unsigned int i = 0; i < gpuProgram->namedUniforms.size(); i++)
    {
        GpuProgram::NamedUniform& u = *gpuProgram->namedUniforms[i];
        switch (u.datatype)
        {
        case VertexArray::FLOAT:
            gpuProgram->setUniformfv(u.varName.c_str(), u.comp_count, (const float*)u.data);
            break;
        case VertexArray::INT:
            gpuProgram->setUniformiv(u.varName.c_str(), u.comp_count, (const int*)u.data);
            break;
        default:
            throw_MagicException("Unsupported Auto Uniform datatype.");
            break;
        }
    }

    // set auto uniforms
    Matrix4 temp4m;
    Matrix4 temp4m2;
    Matrix3 temp3m;
    Vector3 tempp3;
    Scalar tempf;
    for (unsigned int i = 0; i < gpuProgram->autoUniforms.size(); i++)
    {
        GpuProgram::AutoUniform& u = *gpuProgram->autoUniforms[i];
        switch (u.type)
        {
        case GpuProgram::MODEL_MATRIX:                   // mat4
            gpuProgram->setUniformMatrix(u.varName.c_str(), 4, modelMatrix.getArray());
            break;
        case GpuProgram::VIEW_MATRIX:                    // mat4
            gpuProgram->setUniformMatrix(u.varName.c_str(), 4, viewMatrix.getArray());
            break;
        case GpuProgram::PROJECTION_MATRIX:              // mat4
            gpuProgram->setUniformMatrix(u.varName.c_str(), 4, projectionMatrix.getArray());
            break;

            // TODO: stop multiplying these matrices for every individual mesh
        case GpuProgram::MODEL_VIEW_MATRIX:              // mat4
            temp4m.multiply(viewMatrix, modelMatrix);
            gpuProgram->setUniformMatrix(u.varName.c_str(), 4, temp4m.getArray());
            break;
        case GpuProgram::VIEW_PROJECTION_MATRIX:         // mat4
            temp4m.multiply(projectionMatrix, viewMatrix);
            gpuProgram->setUniformMatrix(u.varName.c_str(), 4, temp4m.getArray());
            break;
        case GpuProgram::MODEL_PROJECTION_MATRIX:        // mat4
            temp4m.multiply(projectionMatrix, modelMatrix);
            gpuProgram->setUniformMatrix(u.varName.c_str(), 4, temp4m.getArray());
            break;
        case GpuProgram::MODEL_VIEW_PROJECTION_MATRIX:   // mat4
            temp4m.multiply(viewMatrix, modelMatrix);
            temp4m2.multiply(projectionMatrix, temp4m);
            gpuProgram->setUniformMatrix(u.varName.c_str(), 4, temp4m2.getArray());
            break;
        case GpuProgram::NORMAL_MATRIX:                  // mat3
            temp4m.multiply(viewMatrix, modelMatrix);
            temp4m.extractRotation(temp3m);
            gpuProgram->setUniformMatrix(u.varName.c_str(), 3, temp3m.getArray());
            break;

        case GpuProgram::TEXTURE0:                       // sampler2D
            MAGIC_THROW(material.textures[0] == NULL, "Material has auto-bound "
                "texture set, but no texture set for the index.");
            if (this->useTextures)
                gpuProgram->setTexture(u.varName.c_str(), material.textures[0].get(), 0);
            else
                gpuProgram->setTexture(u.varName.c_str(), fallbackTexture.get(), 0);
            break;
        case GpuProgram::NORMAL_MAP:                       // sampler2D
            if (material.normalMap != nullptr && this->useNormalMaps)
            {
                gpuProgram->setTexture(u.varName.c_str(), material.normalMap.get(), 8);
                gpuProgram->setUniformf("normalMapping", 1.0f);
            }
            else
                gpuProgram->setUniformf("normalMapping", 0.0f);
            break;
        case GpuProgram::SHININESS:                 // float
            gpuProgram->setUniformf(u.varName.c_str(), material.shininess);
            break;
        case GpuProgram::SPECULAR_COLOR:                  // vec3
            gpuProgram->setUniformf(u.varName.c_str(),
                material.specularColor.getChannel(0, true),
                material.specularColor.getChannel(1, true),
                material.specularColor.getChannel(2, true)
                );
            break;



        case GpuProgram::LIGHT_LOCATION:                 // vec4
            tempp3 = light.location;
            tempf = 1.0f;
            if (light.locationLess)
                tempf = 0.0f;
            gpuProgram->setUniformf(u.varName.c_str(), tempp3.x(),
                tempp3.y(), tempp3.z(), tempf);
            break;
        case GpuProgram::LIGHT_DIRECTION:                 // vec3
            tempp3 = light.direction;
            gpuProgram->setUniformf(u.varName.c_str(), tempp3.x(),
                tempp3.y(), tempp3.z());
            break;
        case GpuProgram::LIGHT_ANGLE:                 // float
            gpuProgram->setUniformf(u.varName.c_str(), this->light.angle);
            break;
        case GpuProgram::LIGHT_INTENSITY:                 // float
            gpuProgram->setUniformf(u.varName.c_str(), this->light.intensity);
            break;
        case GpuProgram::LIGHT_ATTENUATION_FACTOR:                 // float
            gpuProgram->setUniformf(u.varName.c_str(), this->light.attenuationFactor);
            break;
        case GpuProgram::LIGHT_AMBIENT_FACTOR:                 // float
            gpuProgram->setUniformf(u.varName.c_str(), this->light.ambientFactor);
            break;
        case GpuProgram::LIGHT_COLOR:                           // vec3
            gpuProgram->setUniformf(u.varName.c_str(), 
                light.lightColor.getChannel(0, true),
                light.lightColor.getChannel(1, true),
                light.lightColor.getChannel(2, true)
            );
            break;
        case GpuProgram::FLAT_PROJECTION:   // mat4
            temp4m.createOrthographicMatrix(0, (Scalar)this->graphics.getDisplayWidth(),
                0, (Scalar)this->graphics.getDisplayHeight(), -1.0, 1.0);
            gpuProgram->setUniformMatrix(u.varName.c_str(), 4, temp4m.getArray());
            break;
        
        default:
            MAGIC_ASSERT(false);
            break;
        }
    }

    // check for a depth lie
    if (material.depthBufferLie)
    {
        glPolygonOffset(material.depthBufferLie, 1.0f);
        glEnable(GL_POLYGON_OFFSET_FILL);
    }

    // disable depth buffer writes if mesh is transparent
    if (material.transparent)
        glDepthMask(GL_FALSE);

    // setup wireframe if set to
    if (wireframe)
    {
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }
}

void World::tearDownMaterial(Material& material, bool wireframe)
{
    // disable depth lie if it was enabled
    if (material.depthBufferLie)
        glDisable(GL_POLYGON_OFFSET_FILL);

    // re-enabled depth buffer write
    if (material.transparent)
        glDepthMask(GL_TRUE);

    // restore after wireframe
    if (wireframe)
    {
        glDisable(GL_LINE_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }
}

void World::renderMesh(Mesh& mesh)
{
    // draw mesh
    mesh.getVertexArray().draw(mesh.getPrimitive(), mesh.getVertexCount());
    vertexCount += mesh.getVertexCount();   
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

	std::vector<Object*> sortedObjects;
	sortedObjects.reserve(this->objects.size());
	//sortedObjects.insert(sortedObjects.begin(), this->objects.begin(), this->objects.end());

    // only render objects that exist in the view frustum of the camera
    auto viewFrustum = camera->getViewFrustum();
    for (Object* o : this->objects)
    {
        auto sphere = o->getModel()->getMeshes()->getBoundingSphere();
        // TODO: add in bounding sphere offset for this to work correctly
        auto loc = o->getPosition().getLocation();
        if (viewFrustum.sphereInFrustum(Vector3(loc.x(), loc.y(), loc.z()), sphere.getRadius()))
            sortedObjects.push_back(o);
        else
            continue;
    }

    std::vector<std::shared_ptr<Object>> sortedStaticObjects;
    sortedStaticObjects.reserve(this->staticObjectCount);

    // only render objects that exist in the view frustum of the camera
    for (auto it : this->staticObjects)
    {
        for (std::shared_ptr<Object> o : *it.second)
        {
            auto sphere = o->getModel()->getMeshes()->getBoundingSphere();
            auto loc = sphere.getOffset();
            if (viewFrustum.sphereInFrustum(Vector3(loc.x(), loc.y(), loc.z()), sphere.getRadius()))
                sortedStaticObjects.push_back(o);
            else
                continue;
        }
    }

	Vector3 loc = camera->getPosition().getLocation();
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

    vertexCount = 0;

    // render static objects (aka scenery)
    Matrix4 identityMatrix;
    Material* material = nullptr;
    for (auto ob : sortedStaticObjects)
    {
        if (material == nullptr || material != ob->getModel()->getMaterial().get())
        {
            if (material != nullptr)
                tearDownMaterial(*material, this->wireframeEnabled);
            material = ob->getModel()->getMaterial().get();
            setupMaterial(*material, identityMatrix, view, projection, this->wireframeEnabled);
        }

        for (auto mesh : *ob->getModel()->getMeshes())
        {
            renderMesh(*mesh);
            if (showNormals)
                renderMesh(mesh->getVisibleNormals());
        }
    }
    if (material != nullptr)
        tearDownMaterial(*material, this->wireframeEnabled);

	// render all objects
	std::vector<Object*>::iterator it = sortedObjects.begin();
	Object* ob;
	for(; it != sortedObjects.end(); it++)
	{
	    // get object and entity
	    ob = (*it);
	    
		const std::shared_ptr<Meshes> meshes = ob->getModel()->getMeshes();
		if (meshes == nullptr)
			break;

        // get mesh and material data
		auto material = ob->getModel()->getMaterial();
            
        // get model/world matrix for object (same for all meshes in object)
        Matrix4 model;
        ob->getPosition().getTransformMatrix(model);
        
        setupMaterial(*material, model, view, projection, this->wireframeEnabled);
		for(const std::shared_ptr<Mesh> mesh : *meshes)
		{   
            renderMesh(*mesh);
            if (showNormals)
                renderMesh(mesh->getVisibleNormals());
		}
        tearDownMaterial(*material, this->wireframeEnabled);
	} // end of all objects

    // render bounding spheres, if requested
    if (this->showBoundingSpheres)
    {
        for (auto it : this->staticObjects)
        {
            auto material = it.first;
            setupMaterial(*material, identityMatrix, view, projection, true);
            for (unsigned int i = 0; i < it.second->size(); i++)
            {
                renderMesh(it.second->at(i)->getModel()->getMeshes()->getBoundingSphereMesh());
            }
            tearDownMaterial(*material, true);
        }

        std::set<Object*>::iterator it2 = this->objects.begin();
        for (; it2 != this->objects.end(); it2++)
        {
            // get object and entity
            ob = (*it2);

            const std::shared_ptr<Meshes> meshes = ob->getModel()->getMeshes();
            if (meshes == nullptr)
                break;

            // get mesh and material data
            auto material = ob->getModel()->getMaterial();

            // get model/world matrix for object (same for all meshes in object)
            Matrix4 model;
            ob->getPosition().getTransformMatrix(model);

            // render bounding sphere
            setupMaterial(*material, model, view, projection, true);
            renderMesh(meshes->getBoundingSphereMesh());
            tearDownMaterial(*material, true);
        } // end of all objects
    }

	// Do the buffer Swap
    graphics.swapBuffers();

	this->renderTimeElapsed = timer.getElapsedTime();
}










};


















