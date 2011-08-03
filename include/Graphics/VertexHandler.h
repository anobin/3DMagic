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
#include "../Util/magic_assert.h"

#include <map>
#include <vector>

namespace Magic3D
{

/** Helps to manage vertex attributes when
 * building and drawing objects.
 */
class VertexHandler
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


private:	
    /// datatype used when building model by hand, before end() call
	struct BuildData
	{
		char* temp;
		int tempLength;
		int currentIndex;
		int components;
		VertexArray::DataTypes type;
		
		inline BuildData(int components, VertexArray::DataTypes type, int vertexCount )
		{
		    this->components = components;
		    this->tempLength = vertexCount * components * VertexArray::getDataTypeSize( type );
		    this->temp = new char[ this->tempLength ];
		    this->type = type;
		    this->currentIndex = 0;
		}
		
		inline ~BuildData()
		{
		    delete[] temp;
		}
		    
	};
    
	/** mapping of attribute build index to build data
	 */
	std::map< int, BuildData* > buildData;
	
	std::map< std::string, int > name2index;
	
	std::map< int, AttributeData* > index2data;
	
	/// vector of attribute data
	std::vector< AttributeData* > attributeData;
	
	/// number of verticies being managed
	int vertexCount;
	
	/// the next index to hand out for a new attribute being used
	int nextIndex;
	
	template< typename T>
	inline BuildData* getData( int index, int components )
	{
	    // try to find entry
	    std::map< int, BuildData* >::iterator it = buildData.find( index );
	    
	    // if no find, exception
	    MAGIC_THROW ( it == buildData.end(), "Attempt to set data for non-existent attribute." );
	    BuildData* d = it->second;
	    
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
	 */
	VertexHandler();
	
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
	    BuildData* data = this->getData<T>( id, 4 );
	    T* d = (T*) data->temp;
		d[ data->currentIndex   ] = c1;
		d[ data->currentIndex+1 ] = c2;
		d[ data->currentIndex+2 ] = c3;
		d[ data->currentIndex+3 ] = c4;
		data->currentIndex += 4;
	}
	
	template< typename T >
	inline void setAttribute3(int id,
							   T c1, T c2, T c3 )
	{		
	    BuildData* data = this->getData<T>( id, 3 );
	    T* d = (T*) data->temp;
		d[ data->currentIndex   ] = c1;
		d[ data->currentIndex+1 ] = c2;
		d[ data->currentIndex+2 ] = c3;
		data->currentIndex += 3;
	}
	
	template< typename T >
	inline void setAttribute2(int id,
							   T c1, T c2 )
	{		
	    BuildData* data = this->getData<T>( id, 2 );
	    T* d = (T*) data->temp;
		d[ data->currentIndex   ] = c1;
		d[ data->currentIndex+1 ] = c2;
		data->currentIndex += 2;
	}
	
	template< typename T >
	inline void setAttributeN(int id, int comps,
							   T* c )
	{		
	    BuildData* data = this->getData<T>( id, comps );
	    T* d = (T*) data->temp;
	    for( int i=0; i < comps; i++ )
	        (*d)[ data->currentIndex+i ] = c[ i ];
		data->currentIndex += comps;
	}
	
	/** Get the id for a attribute
	 * @param name the name of the attribute to get the id for
	 * @return id of attribute
	 */
	inline int getAttribId( const char* name, int components, VertexArray::DataTypes type )
	{	    
	    int index;
	    
	    // ensure begin was called
	    MAGIC_THROW( this->vertexCount <= 0, "Attempt to get attribute id before begin() was called" );
	    
	    // try to find current entry
	    std::map< std::string, int >::iterator it = name2index.find( std::string( name ) );
	    
	    // if no entry, create new
	    if ( it == name2index.end() )
	    {
	        index = nextIndex;
	        nextIndex++;
	        
	        // add attribute data
            AttributeData* d = new AttributeData( name, components, type );
            attributeData.push_back( d );
            index2data.insert( std::pair< int, AttributeData* >( index, d ) );
            
            // add temporary build data
            BuildData* b = new BuildData( components, type, this->vertexCount );
            buildData.insert( std::pair< int, BuildData* >( index, b ) );
            name2index.insert( std::pair< std::string, int >( std::string(name), index ) ); 
        }
        else
            index = it->second;
	    
	    return index;
	}
	
	
	/** end a vertex building sequence
	 */
	void end();

	inline std::vector< AttributeData* >& getAttributeData()
	{
	     return attributeData;   
	}
	
	inline int getVertexCount()
	{
	    return vertexCount;
	}

};


};




#endif
















