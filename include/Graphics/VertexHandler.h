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
/** Header file for VertexHandler class
 *
 * @file VertexHandler.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VERTEX_HANDLER_H
#define MAGIC3D_VERTEX_HANDLER_H

#include "../Exceptions/MagicException.h"
#include "../Shaders/VertexAttribSpec.h"
#include "../Shaders/Shader.h"
#include "../Graphics/VertexArray.h"
#include "../Util/Color.h"
#include "../Math/Math.h"
#include "../Util/magic_throw.h"

#include <map>

namespace Magic3D
{

/** Helps to manage vertex attributes when
 * building and drawing objects.
 */
class VertexHandler
{
private:
	/// data type for storage
	struct AttributeData
	{
		int index;
		Buffer* buffer;
		char* temp;
		int tempLength;
		int currentVertex;
		int components;
		VertexArray::DataTypes type;
		
		inline AttributeData( int index, int components, VertexArray::DataTypes type,
		    int vertexCount )
		{
		    this->index = index;
		    this->buffer = NULL;
		    this->components = components;
		    this->tempLength = vertexCount * components * VertexArray::getDataTypeSize( type );
		    this->temp = new char[ this->tempLength ];
		    this->type = type;
		    this->currentVertex = 0;
		}
		
		inline ~AttributeData()
		{
		    delete buffer;
		    delete[] temp;
		}
		    
	};
	
	/** mapping of attribute data, filled by getBuiltInAttribId() and
	 * getNamedAttribId(). Maps GL index to attribute data.
	 */
	std::map< int, AttributeData* > data;
	
	/// vertex array that is handled
	VertexArray array;
	
	/// number of verticies being managed
	int vertexCount;
	
	/// vertex attribute specification used
	const VertexAttribSpec* spec;
	
	/// local cache of indices for built-in attributes, used in convenience methods
    int builtin_index[ (int) VertexAttribSpec::BUILTIN_ATTRIB_COUNT ];
	
	template< typename T>
	inline AttributeData* getData( int index, int components )
	{
	    // try to find entry
	    std::map< int, AttributeData* >::iterator it = data.find( index );
	    
	    // if no find, exception
	    MAGIC_THROW ( it == data.end(), "Attempt to set data for non-existent attribute." );
	    AttributeData* d = it->second;
	    
	    // if type size mismatch, exception
	    MAGIC_THROW ( VertexArray::getDataTypeSize( d->type ) != sizeof( T ), 
	        "Attempt to set data for attribute with improper type." );
	    
	    // if components mismatch, exception
	    MAGIC_THROW ( d->components != components, "Attempt to set data for attribute "
	        "with improper component number." );
	    
	    return d;
	}
	

public:
	/** Standard Constructor
	 * @param spec the shader-vertex interface spec to use
	 */
	VertexHandler(const VertexAttribSpec* spec);
	
	/// destructor
	~VertexHandler();
	
	/** Starts a vertex building sequence
	 * @param vertexCount the number of verticies to be handled
	 */
	void begin(int vertexCount);
	
	template< typename T >
	inline void setAttribute4(int id,
							   T c1, T c2, T c3, T c4)
	{		
	    AttributeData* data = this->getData<T>( id, 4 );
	    T* d = (T*) data->temp;
		d[ data->currentVertex   ] = c1;
		d[ data->currentVertex+1 ] = c2;
		d[ data->currentVertex+2 ] = c3;
		d[ data->currentVertex+3 ] = c4;
		data->currentVertex += 4;
	}
	
	template< typename T >
	inline void setAttribute3(int id,
							   T c1, T c2, T c3 )
	{		
	    AttributeData* data = this->getData<T>( id, 3 );
	    T* d = (T*) data->temp;
		d[ data->currentVertex   ] = c1;
		d[ data->currentVertex+1 ] = c2;
		d[ data->currentVertex+2 ] = c3;
		data->currentVertex += 3;
	}
	
	template< typename T >
	inline void setAttribute2(int id,
							   T c1, T c2 )
	{		
	    AttributeData* data = this->getData<T>( id, 2 );
	    T* d = (T*) data->temp;
		d[ data->currentVertex   ] = c1;
		d[ data->currentVertex+1 ] = c2;
		data->currentVertex += 2;
	}
	
	template< typename T >
	inline void setAttributeN(int id, int comps,
							   T* c )
	{		
	    AttributeData* data = this->getData<T>( id, comps );
	    T* d = (T*) data->temp;
	    for( int i=0; i < comps; i++ )
	        (*d)[ data->currentVertex+i ] = c[ i ];
		data->currentVertex += comps;
	}
	
	
	
	/** Get the id for a built-in attribute
	 * @param attribute the attribute to get the id for
	 * @return id of attribute, -1 for not supported
	 */
	inline int getBuiltInAttribId( VertexAttribSpec::BuiltInAttributeType attribute )
	{
	    int index = spec->getBuiltInAttrib( attribute );
	    if ( index < 0 )
	        return index; // attribute not supported by shader :(
	    
	    // try to find current entry
	    std::map< int, AttributeData* >::iterator it = data.find( index );
	    
	    // if no entry, create new
	    if ( it == data.end() )
	    {
            AttributeData* d;
            switch( attribute )
            {
                case VertexAttribSpec::POSITION:
                    d = new AttributeData( index, 4, VertexArray::FLOAT, vertexCount );
                    break;
                case VertexAttribSpec::NORMAL:
                    d = new AttributeData( index, 3, VertexArray::FLOAT, vertexCount );
                    break;
                case VertexAttribSpec::COLOR:
                    d = new AttributeData( index, 4, VertexArray::FLOAT, vertexCount );
                    break;
                case VertexAttribSpec::BASE_TEXTURE:
                    d = new AttributeData( index, 2, VertexArray::FLOAT, vertexCount );
                    break;
                    
                default:
                    throw MagicException( "Internal error" );   
            }
            data.insert( std::pair< int, AttributeData* >( index, d ) );
        }
	    
	    return index;
	}
	
	/** Get the id for a named attribute
	 * @param name the name of the attribute to get the id for
	 * @return id of attribute, -1 for not supported
	 */
	inline int getNamedAttribId( const char* name )
	{
	    const VertexAttribSpec::NamedAttribType* a = spec->getNamedAttrib( name );
	    if ( a->index < 0 )
	        return a->index; // attribute not supported by shader :(
	    
	    // try to find current entry
	    std::map< int, AttributeData* >::iterator it = data.find( a->index );
	    
	    // if no entry, create new
	    if ( it == data.end() )
	    {
            AttributeData* d = new AttributeData( a->index, a->components, a->type, vertexCount );
            data.insert( std::pair< int, AttributeData* >( a->index, d ) );
        }
	    
	    return a->index;
	}
	
	
	inline void position3f( float x, float y, float z )
	{
	    this->position4f( x, y, z, 1.0f );
	}
	
	
	inline void position4f( float x, float y, float z, float w )
	{
	    int id = builtin_index[ (int) VertexAttribSpec::POSITION  ];
	    
	    // lazy instantionation
	    if (id == -1)
	    {
	        id = this->getBuiltInAttribId( VertexAttribSpec::POSITION );
	        if ( id == -1 )
	            throw MagicException( "VertexHandler::position[3|4]f called without position support in shader" );
	    }
	    
	    this->setAttribute4<float>( id, x, y, z, w );
	}
	
	inline void normal3f( float x, float y, float z )
	{
	    int id = builtin_index[ (int) VertexAttribSpec::NORMAL  ];
	    
	    // lazy instantionation
	    if (id == -1)
	    {
	        id = this->getBuiltInAttribId( VertexAttribSpec::NORMAL );
	        if ( id == -1 )
	            throw MagicException( "VertexHandler::normal3f called without normal support in shader" );
	    }
	    
	    this->setAttribute3<float>( id, x, y, z );
	}
	
	inline void texCoord2f( float x, float y )
	{
	    int id = builtin_index[ (int) VertexAttribSpec::BASE_TEXTURE  ];
	    
	    // lazy instantionation
	    if (id == -1)
	    {
	        id = this->getBuiltInAttribId( VertexAttribSpec::BASE_TEXTURE );
	        if ( id == -1 )
	            throw MagicException( "VertexHandler::texCoord2f called without base texture support in shader" );
	    }
	    
	    this->setAttribute2<float>( id, x, y );
	}
	
	inline void color4f( float r, float g, float b, float a )
	{
	    int id = builtin_index[ (int) VertexAttribSpec::COLOR  ];
	    
	    // lazy instantionation
	    if (id == -1)
	    {
	        id = this->getBuiltInAttribId( VertexAttribSpec::COLOR );
	        if ( id == -1 )
	            throw MagicException( "VertexHandler::color4f called without position support in shader" );
	    }
	    
	    this->setAttribute4<float>( id, r, g, b, a );
	}
	
	
	/** end a vertex building sequence
	 */
	void end();
	
	/// draw all verticies
	inline void draw(VertexArray::Primitives primitive = VertexArray::TRIANGLES)
	{
#ifdef MAGIC3D_NO_VERTEX_ARRAYS
		std::map<int, AttributeData*>::iterator it =
		data.begin();
		for(; it != data.end(); it++)
		{
			array.setAttributeArray(it->second->index, it->second->components,
								it->second->type, *it->second->buffer);
		}
#endif

		array.draw(primitive, vertexCount);

#ifdef MAGIC3D_NO_VERTEX_ARRAYS
		it = data.begin();
		for(; it != data.end(); it++)
		{
			glDisableVertexAttribArray(it->second->index);
		}
#endif
	}










};


};




#endif
















