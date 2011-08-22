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
#include <Cameras/FPCamera.h>

extern Magic3D::FPCamera camera;
extern Magic3D::Texture* marbleTex;

namespace Magic3D
{
    


void World::processFrame()
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
    
    // Clear the color and depth buffers
	graphics.clearDisplay();

	// render all objects
	std::set<Object*>::iterator it = objects.begin();
	Object* ob;
	GraphicalEntity* en;
	const Model* model;
	const Mesh* mesh;
	const Mesh** meshes;
	const Material** materials;
	int meshCount;
	const Material* material;
	VertexArray* array;
	const Mesh::AttributeData* adata;
	int attributeCount;
	int vertexCount;
	Shader* shader; 
	for(; it != objects.end(); it++)
	{
	    // get object and entity
	    ob = (*it);
	    en = ob->getGraphical();
	    
	    // ensure there is a graphical part to this entity
	    if (en == NULL)
	        continue; // no graphical part to render
	    
	    // get model
	    model = en->getModel();
	    MAGIC_ASSERT(model != NULL);
	    
	    // get mesh and material data
	    meshes = model->getMeshData();
	    materials = model->getMaterialData();
	    meshCount = model->getMeshCount();
	    
	    // ensure there is at least one mesh
	    MAGIC_ASSERT(meshCount > 0);
	    
	    // render each mesh
	    for(int i=0; i < meshCount; i++)   
	    {
	        // get mesh and material
	        mesh = meshes[i];
	        material = materials[i];
	        
	        // ensure there is a shader
	        shader = material->shader;
	        MAGIC_ASSERT(shader != NULL);
	        
	        // get mesh data
	        adata = mesh->getAttributeData();
	        attributeCount = mesh->getAttributeCount();
	        vertexCount = mesh->getVertexCount();
	        
	        // get camera matrix
	        Matrix4 cameraMatrix;
	        camera.getPosition().getCameraMatrix(cameraMatrix);
	        
	        // get tranform (model-view) matrix from position (model) and camera (view) matrices 
            Matrix4 positionMatrix;
            Matrix4 transformMatrix;
            ob->getPosition().getTransformMatrix(positionMatrix);
            transformMatrix.multiply(cameraMatrix, positionMatrix);
            
            // create mvp matrix from projection and transform (model-view)
            Matrix4 mvp;
            mvp.multiply(camera.getProjectionMatrix(), transformMatrix);
            
            // extract normal matrix from transform matrix
            Matrix3 normal;
            transformMatrix.extractRotation(normal);
            
            // transform the light position into eye coordinates
            Point3 lightPos(0.0f, 1000.0f, 0.0f );
            lightPos.transform(cameraMatrix);
            
            // 'use' shader
	        shader->use();
	        
	        // set named uniforms
	        shader->setUniformf(  "lightPosition", lightPos.getX(), lightPos.getY(), lightPos.getZ() );
	        shader->setUniformf( "skyColor", 1.0f, 1.0f, 1.0f);
	        shader->setUniformf( "groundColor", 0.1f, 0.1f, 0.1f);
	        
	        // set auto uniforms
	        shader->setUniformMatrix( "mvMatrix",     4, transformMatrix.getArray() );
	        shader->setUniformMatrix( "mvpMatrix",    4, mvp.getArray()             );
	        shader->setUniformMatrix( "normalMatrix", 3, normal.getArray()          );
	        shader->setTexture( "textureMap", material->textures[0] );
	        
	        
	        // bind vertexArray
	        array = new VertexArray();
	        for(int j=0; j < attributeCount; j++)
	        {
	            int bind = shader->getAttribBinding( 
	                Mesh::attributeTypeNames[(int)adata[j].type] );
	            if (bind < 0) // shader does not have attribute
	                continue; 
	            array->setAttributeArray(bind, Mesh::attributeTypeCompCount[(int)adata[j].type],
	                VertexArray::FLOAT, adata[j].buffer);
	        }
	        
	        // draw mesh
	        array->draw(material->primitive, vertexCount);
	        
	        // delete bound array
	        delete array;
	    }   
	}
	
	// Do the buffer Swap
    graphics.swapBuffers();
}










};


















