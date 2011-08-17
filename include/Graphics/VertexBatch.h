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
/** Header file for VertexBatch class
 *
 * @file VertexBatch.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VERTEX_BATCH_H
#define MAGIC3D_VERTEX_BATCH_H

#include "../Exceptions/MagicException.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Texture.h"

#include <vector>
#include <string.h>

namespace Magic3D
{
    
class VertexBatch;
class VertexBatchBuilder;

/** Contains the attribute data for a batch of vertices as well as properties,
 * such as texture, that are used to setup the shader to render the vertices.
 */
class VertexBatch
{
public:
	/// data for a attribute, ready to be bound to a shader
	struct AttributeData
	{
	    /// attribute data in a buffer on graphics memory
		Buffer* buffer;
		/// number of components in attribute
		int components;
		/// type of the attribute
		VertexArray::DataTypes type;
		/// name of the attribute in shader
		char* name;
		
	private:
	    friend class VertexBatch;
	    friend class VertexBatchBuilder;
	    
		inline AttributeData( const char* name, int components, VertexArray::DataTypes type )
		{
		    this->buffer = NULL;
		    this->components = components;
		    this->type = type;
		    this->name = new char[ strlen( name ) + 1 ];
		    strcpy( this->name, name );
		}
		
		inline ~AttributeData()
		{
		    delete buffer;
		    delete[] name;
		}
		    
	};
	
	/// types of optional data this batch may contain
	enum PropertyIndex
	{
	    TEXTURE,            // texture for this batch, pointer
	    RENDER_OFFSET,      // purely graphical offset to counter z-fighting, float
	    PROPERTY_COUNT
	};


private:	
    friend class VertexBatchBuilder;
    
    /// property entry type
    struct PropertyEntry
    {
        bool set;
        union 
        {
            float u_float;
            Texture* u_texture;
        } data;
        
        inline PropertyEntry(): set(false) {}
    };
    
	/// vector of attribute data
	std::vector< AttributeData* > attributeData;
	
	/// number of verticies being managed
	int vertexCount;
	
	/// the properties set for the system
	PropertyEntry properties[PROPERTY_COUNT];
	
public:
    /// Standard Constructor
	inline VertexBatch(): vertexCount(0) {}
    
	/// destructor
	~VertexBatch();

	inline std::vector< AttributeData* >& getAttributeData()
	{
	     return attributeData;   
	}
	
	inline int getVertexCount()
	{
	    return vertexCount;
	}
	
	inline bool isPropertySet(PropertyIndex p)
	{
	    return properties[(int)p].set;
	}
	
	template<class T>
	inline T getProperty(PropertyIndex p)
	{
	    throw_MagicException( "Attempt to get property with unsupported type" );
	}
	
	template<class T>
	inline void setProperty(PropertyIndex p, T t)
	{
	    throw_MagicException( "Attempt to get property with unsupported type" );
	}
	
	
};

template<> 
inline float VertexBatch::getProperty<float>(PropertyIndex p)
{
    return properties[(int)p].data.u_float;
}


template<>
inline Texture* VertexBatch::getProperty<Texture*>(PropertyIndex p)
{
    return properties[(int)p].data.u_texture;
}

template<>
inline void VertexBatch::setProperty<float>(PropertyIndex p, float f)
{
    properties[(int)p].data.u_float = f;
    properties[(int)p].set = true;
}

template<>
inline void VertexBatch::setProperty<Texture*>(PropertyIndex p, Texture* t)
{
    properties[(int)p].data.u_texture = t;
    properties[(int)p].set = true;
}



};




#endif
















