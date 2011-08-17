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

#include <vector>

namespace Magic3D
{

class GraphicalBody;

/** Graphical body of an object. Contains graphical properties
 * (texture, position, model).
 */
class GraphicalBody
{
public:
    struct RenderBatch
    {
    private:
        friend class GraphicalBody;
        VertexArray array;
        VertexBatch* batch;
        int vertexCount;
        
    public:
        
        inline bool isPropertySet(VertexBatch::PropertyIndex p)
        {
            return batch->isPropertySet(p);
        }
        
        template<class T>
        inline T getProperty(VertexBatch::PropertyIndex p)
        {
            return batch->getProperty<T>(p);
        }
        
        template<class T>
        inline void setProperty(VertexBatch::PropertyIndex p, T t)
        {
            batch->setProperty<T>(p, t);
        }
        
        inline void draw( VertexArray::Primitives primitive )
        {
            array.draw(primitive, vertexCount);
        }
        
    };
    
protected:
	/// model to use to represent this object
	Model& model;
	
	/// shader to use to render this body
	Shader* shader;
	
	std::vector<RenderBatch*> data;
	
public:
	/// standard constructor
	inline GraphicalBody(Model& model): model(model), shader(NULL) {}
	
	/// destructor
	virtual ~GraphicalBody();
	
	/// get the model used
	inline Model& getModel()
	{
		return model;
	}
	
	/// set the shader to render with
	inline void setShader( Shader& shader )
	{
	    const VertexAttribSpec::AttribType* attrib;
	    
	    // clear out any old vertex arrays
	    std::vector<RenderBatch*>::iterator ait;
	    for (ait = data.begin(); ait != data.end(); ait++)
	        delete (*ait);
	    data.clear();
	    
	    // create bindings in vertex array for data in model to variables in shader
	    const VertexAttribSpec* spec  = shader.getVertexAttribSpec();
	    std::vector< VertexBatch* >& batch = model.getBatchData();
	    std::vector< VertexBatch*>::iterator b_it = batch.begin();
	    for(; b_it != batch.end(); b_it++ )
	    {
	    
            std::vector< VertexBatch::AttributeData* >& data = (*b_it)->getAttributeData();
            std::vector< VertexBatch::AttributeData* >::iterator it = data.begin();
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
	}
	
	/// get the shader to render with
	inline Shader* getShader()
	{
	    return this->shader;    
	}
	
	inline std::vector<RenderBatch*>& getRenderData()
	{
	    return this->data;
	}

};





};



#endif





















