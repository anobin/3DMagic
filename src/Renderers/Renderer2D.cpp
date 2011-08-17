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
/** Implementation file for Renderer2D class
 *
 * @file HemRenderer2D.cpp
 * @author Andrew Keating
 */

#include <Renderers/Renderer2D.h>


namespace Magic3D
{
    
/// standard constructor
Renderer2D::Renderer2D(ResourceManager& resourceManager): shader(NULL), camera(NULL)
{
    // create shader that will be used
    Handle<TextResource> vp = resourceManager.get<TextResource>("shaders/Shader2D.vp");
    Handle<TextResource> fp = resourceManager.get<TextResource>("shaders/Shader2D.fp");
	shader = new Shader( vp()->getText(), fp()->getText() );
	shader->bindAttrib( "Location", "location", 4, VertexArray::FLOAT );
	shader->bindAttrib( "TexCoord", "texCoord", 2, VertexArray::FLOAT );
	shader->link();
}
    

/// destructor
Renderer2D::~Renderer2D()
{
    delete shader;
}

unsigned int Renderer2D::getPassCount()
{
    return 1; // only ever one pass
}

void Renderer2D::setup (unsigned int pass)
{
    // verify that we have valid data
    MAGIC_ASSERT( shader      != NULL ); // assert becuase we should have created shader
    MAGIC_THROW( camera == NULL, "Attempt to render without camera" );
                                      
    // 'use' shader
	shader->use();
	
	// set uniforms that are the same for all objects
	shader->setUniformMatrix( "mvpMatrix", 4, camera->getProjectionMatrix().getArray() );
}

void Renderer2D::render( Object* object, unsigned int pass )
{                                      
    // 'use' shader
	shader->use();
	
    // get graphical body for object
    GraphicalBody& body = object->getGraphical();
    
    // ensure the body's vertex data is bound to this shader's variables
    if ( body.getShader() != this->shader )
        body.setShader( *this->shader );
    
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
        
        // draw batch
        b.draw(VertexArray::TRIANGLE_FAN);
    
    }
    
}


};











