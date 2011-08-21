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
/** Header file for GraphicalEntity class
 *
 * @file GraphicalEntity.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_GRAPHICAL_ENTITY_H
#define MAGIC3D_GRAPHICAL_ENTITY_H

#include "Model.h"
#include "../Math/Position.h"
#include "../Util/Color.h"
#include "Texture.h"
#include "VertexArray.h"
#include "../Exceptions/MagicException.h"
#include "../Shaders/Shader.h"

#include <vector>

namespace Magic3D
{

class GraphicalEntity;

/** Graphical body of an object. Contains graphical properties
 * (texture, position, model).
 */
class GraphicalEntity
{    
protected:
	/// model to use to represent this entity
	Model* model;
	
public:
	/// standard constructor
	inline GraphicalEntity(Model* model): model(model) {}
	
	/// destructor
	virtual ~GraphicalEntity();
	
	/// get the model used
	inline Model* getModel()
	{
		return model;
	}
	
	/// set the shader to render with
	/*inline void setShader( Shader& shader )
	{
	    const VertexAttribSpec::AttribType* attrib;
	    
	    // clear out any old vertex arrays
	    std::vector<RenderBatch*>::iterator ait;
	    for (ait = data.begin(); ait != data.end(); ait++)
	        delete (*ait);
	    data.clear();
	    
	    // create bindings in vertex array for data in model to variables in shader
	    const VertexAttribSpec* spec  = shader.getVertexAttribSpec();
	    std::vector< Mesh* >& batch = model.getBatchData();
	    std::vector< Mesh*>::iterator b_it = batch.begin();
	    for(; b_it != batch.end(); b_it++ )
	    {
	    
            std::vector< Mesh::AttributeData* >& data = (*b_it)->getAttributeData();
            std::vector< Mesh::AttributeData* >::iterator it = data.begin();
            RenderBatch* a = new RenderBatch();
            for(; it != data.end(); it++)
            {
                // get the data for this attribute in the spec
                attrib = spec->getAttrib( (*it)->name );
                
                // ensure the shader attrib matches the model attribute
                MAGIC_THROW( attrib == NULL || attrib->components != (*it)->components || attrib->type != (*it)->type, 
                    "Attempt to set shader for a graphical body with a model that the shader does not support." );
                
                // bind buffer to shader attrib index
                a->array.setAttributeArray(attrib->index, attrib->components, attrib->type, 
                    *(*it)->buffer);
            }
            a->batch = (*b_it);
            a->vertexCount = (*b_it)->getVertexCount();
            this->data.push_back(a);
            
        }
	     
	    // set the shader for the body
	    this->shader = &shader;
	}*/

};





};



#endif





















