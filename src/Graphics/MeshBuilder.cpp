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
/** Implementation file for MeshBuilder class
 * 
 * @file MeshBuilder.cpp
 * @author Andrew Keating
 */

#include <Graphics/MeshBuilder.h>

namespace Magic3D
{
	
/** Standard Constructor
 * @param spec the shader-vertex interface spec to use
 */
MeshBuilder::MeshBuilder(): batch(NULL), vertexCount(0)
{
    for (int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
        buildData[i] = NULL;
}
	
/// destructor
MeshBuilder::~MeshBuilder()
{
	for (int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
        delete buildData[i];
}
	
/** Starts a vertex building sequence
 * @param vertexCount the number of verticies to be handled
 */
void MeshBuilder::begin(int vertexCount, Mesh* batch)
{
	MAGIC_THROW( this->vertexCount != 0, "Attempt to rebuild vertex handler");
	MAGIC_THROW( vertexCount <= 0, "Invalid vertex count given to begin()");	
	
	this->batch = batch;
	this->vertexCount = vertexCount;
}

/** end a vertex building sequence
 */
void MeshBuilder::end()
{
    int attributeCount = 0;
    
    // count attributes
	for(int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
	{
	    if (this->buildData[i] != NULL)
	        attributeCount++;
	}
	
	// allocate mesh
	batch->allocate( this->vertexCount, attributeCount );
    
	// place all temporary data into graphics memory
	for(int i=0, a=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
	{
	    BuildData* b = this->buildData[i];
	    if (b == NULL)
	        continue; // skip it if attribute is not in mesh
	    
	    MAGIC_THROW(b->currentVertex != this->vertexCount, "Called end() without setting "
	        "all attributes for specified number of vertices." );
	    
	    // setup this attribute
	    Mesh::AttributeData* data =  &batch->attributeData[a];
	    data->_type = (Mesh::AttributeType) i;
		data->_buffer.allocate(b->tempLength, b->temp, Buffer::STATIC_DRAW );
        
		// increment to next attribute
        a++;
	}
	
	// free all temporary build data
	for (int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
	{
        delete buildData[i];
        buildData[i] = NULL;
    }
    this->batch = NULL;	
    this->vertexCount = 0;
}
	






































	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};