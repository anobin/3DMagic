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
/** Implementation file for HemRenderer class
 *
 * @file HemRenderer.cpp
 * @author Andrew Keating
 */

#include <Renderers/HemRenderer.h>


namespace Magic3D
{
    
/// standard constructor
HemRenderer::HemRenderer(ResourceManager& resourceManager): camera( NULL )
{
    // create shader that will be used
    Handle<TextResource> vp = resourceManager.get<TextResource>("shaders/HemisphereTexShader.vp");
    Handle<TextResource> fp = resourceManager.get<TextResource>("shaders/HemisphereTexShader.fp");
	shader = new Shader( vp()->getText(), fp()->getText() );
	shader->bindAttrib( "Position", "vVertex", 4, VertexArray::FLOAT );
	shader->bindAttrib( "Normal", "vNormal", 3, VertexArray::FLOAT );
	shader->bindAttrib( "TexCoord", "vTexCoord0", 2, VertexArray::FLOAT );
	shader->link();
}
    

/// destructor
HemRenderer::~HemRenderer()
{
    delete shader;
}

unsigned int HemRenderer::getPassCount()
{
    return 1; // only ever one pass    
}

void HemRenderer::setup(unsigned int pass)
{
    // we don't care about pass number in this renderer
    
    // verify that we have valid data
    MAGIC_ASSERT( shader      != NULL ); // assert becuase we should have created shader
    MAGIC_THROW( camera == NULL,         // throw becuase 'they' should have given us a camera 
        "Attempt to render without a camera set." );
                                      
    // get camera matrix
    camera->getPosition().getCameraMatrix(cameraMatrix);

    // Transform the light position into eye coordinates
    lightPos.set( lightSource );
	lightPos.transform(cameraMatrix);
	
	// 'use' shader
    shader->use();
	
	// set shader uniforms that are the same for all bodies
	shader->setUniformf(    "lightPosition", lightPos.getX(), lightPos.getY(), lightPos.getZ() );
    shader->setUniformfv(   "skyColor",         3, skyColor.getInternal()     );
    shader->setUniformfv(   "groundColor",      3, groundColor.getInternal()  );
}

void HemRenderer::render( Object* object, unsigned int pass )
{	
    // we don't care about pass number in this renderer
    
	// 'use' shader
    shader->use();
    
    // get graphical body for object
    GraphicalBody& body = object->getGraphical();
    
    // get tranform (model-view) matrix from position (model) and camera (view) matrices 
    Matrix4 positionMatrix;
    Matrix4 transformMatrix;
    object->getPosition().getTransformMatrix(positionMatrix);
    transformMatrix.multiply(cameraMatrix, positionMatrix);
    
    // ensure the body's vertex data is bound to this shader's variables
    if ( body.getShader() != this->shader )
        body.setShader( *this->shader );
    
    // create mvp matrix from projection and transform (model-view)
    Matrix4 mvp;
    mvp.multiply(camera->getProjectionMatrix(), transformMatrix);
    
    // extract normal matrix from transform matrix
    Matrix3 normal;
    transformMatrix.extractRotation(normal);
    
    // set uniforms that are different per body
    shader->setUniformMatrix( "mvMatrix",         4, transformMatrix.getArray()          );
    shader->setUniformMatrix( "mvpMatrix",        4, mvp.getArray()                      );
    shader->setUniformMatrix( "normalMatrix",     3, normal.getArray()                   );
    
    // render every batch in the body
    std::vector<GraphicalBody::RenderBatch*>& batch = body.getRenderData();
    std::vector<GraphicalBody::RenderBatch*>::iterator it = batch.begin();
    for(; it != batch.end(); it++)
    {
        GraphicalBody::RenderBatch& b = *(*it);
        
        // set texture
        MAGIC_THROW( !b.isPropertySet(VertexBatch::TEXTURE),
            "Attempt to render graphical body whose model does not have a texture set." );
        Texture* tex = b.getProperty<Texture*>(VertexBatch::TEXTURE);
        shader->setTexture( "textureMap", tex );
        
        // draw body
        b.draw(VertexArray::TRIANGLES);
        
    }
}


};











