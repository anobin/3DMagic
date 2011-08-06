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
/** Header file for GraphicalBody class
 *
 * @file GraphicalBody.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_GRAPHICAL_BODY_H
#define MAGIC3D_GRAPHICAL_BODY_H

#include "../Models/Model.h"
#include "../Math/Position.h"
#include "../Util/Color.h"
#include "Texture.h"
#include "VertexArray.h"
#include "../Exceptions/MagicException.h"
#include "../Shaders/Shader.h"

namespace Magic3D
{


/** Graphical body of an object. Contains graphical properties
 * (texture, position, model).
 */
class GraphicalBody
{
protected:
	/// model to use to represent this object
	Model& model;
	
	/// shader to use to render this body
	Shader* shader;
	
	/** Vertex array for body, binds attribute data on video memory to shader variables,
	 * created for each ( shader, model ) pair when the shader is set using setShader
	 */
	VertexArray* array;
	
	/// base texture of object
	Texture* baseTexture;
	
	/// adjustment matrix applied before every render
	Matrix4 adjustmentMatrix;

	
public:
	/// standard constructor
	inline GraphicalBody(Model& model): model(model), shader(NULL), array(NULL),
	    baseTexture(NULL) {}
	
	/// destructor
	virtual ~GraphicalBody();


	/** Draw the object
	 */
	inline void draw(VertexArray::Primitives primitive = VertexArray::TRIANGLES)
	{
	    if (array) // TODO
	        array->draw( primitive, model.getVertexCount() );
	}
	
	/// get the model used
	inline Model& getModel()
	{
		return model;
	}
	
	/// set the shader to render with
	inline void setShader( Shader& shader )
	{
	    const VertexAttribSpec::AttribType* attrib;
	    delete array;
	    array = new VertexArray();
	    
	    // create bindings in vertex array for data in model to variables in shader
	    const VertexAttribSpec* spec  = shader.getVertexAttribSpec();
	    std::vector< VertexHandler::AttributeData* >& data = model.getAttributeData();
	    std::vector< VertexHandler::AttributeData* >::iterator it = data.begin();
	    for(; it != data.end(); it++)
	    {
	        // get the data for this attribute in the spec
	        attrib = spec->getAttrib( (*it)->name );
	        
	        // ensure the shader attrib matches the model attribute
	        MAGIC_THROW( attrib == NULL || attrib->components != (*it)->components || attrib->type != (*it)->type, 
	            "Attempt to set shader for a graphical body with a model that the shader does not support." );
	        
	        // bind buffer to shader attrib index
	        array->setAttributeArray(attrib->index, attrib->components, attrib->type, 
	            *(*it)->buffer);
	    }
	     
	    // set the shader for the body
	    this->shader = &shader;
	}
	
	/// get the shader to render with
	inline Shader* getShader()
	{
	    return this->shader;    
	}
	
	/// set the texture
	inline void setBaseTexture(Texture& texture)
	{
		this->baseTexture = &texture;
	}
	
	/// get the texture
	inline Texture* getBaseTexture()
	{
		return this->baseTexture;
	}
	
	inline Matrix4& getAdjustmentMatrix()
	{
	    return adjustmentMatrix;
	}

};





};



#endif





















