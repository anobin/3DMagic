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
/** Implementation file for VertexBatchBuilder class
 * 
 * @file VertexBatchBuilder.cpp
 * @author Andrew Keating
 */

#include <Graphics/VertexBatchBuilder.h>

namespace Magic3D
{
	
/** Standard Constructor
 * @param spec the shader-vertex interface spec to use
 */
VertexBatchBuilder::VertexBatchBuilder(): batch(NULL), vertexCount(0), nextIndex( 0 )
{
}
	
/// destructor
VertexBatchBuilder::~VertexBatchBuilder()
{
	std::map< int, BuildData* >::iterator it =  buildData.begin();
	for(; it != buildData.end(); it++)
		delete it->second;
}
	
/** Starts a vertex building sequence
 * @param vertexCount the number of verticies to be handled
 */
void VertexBatchBuilder::begin(int vertexCount, VertexBatch* batch)
{
	MAGIC_THROW( this->vertexCount != 0, "Attempt to rebuild vertex handler");
	MAGIC_THROW( vertexCount <= 0, "Invalid vertex count given to begin()");	
	
	this->batch = batch;
	this->batch->vertexCount = vertexCount;
	this->vertexCount = vertexCount;
}

/** end a vertex building sequence
 */
void VertexBatchBuilder::end()
{
	// place all temporary data into graphics memory
	std::map< int, BuildData* >::iterator temp_it;
	for(int i=0; i < nextIndex; i++)
	{
	    temp_it = buildData.find( i );
	    MAGIC_ASSERT( temp_it != buildData.end() );
	    BuildData* b = temp_it->second;
		index2data.find( i )->second->buffer = new Buffer(b->tempLength,
            b->temp, Buffer::STATIC_DRAW );
	}
	
	// free all temporary build data
	std::map< int, BuildData* >::iterator it2 =  buildData.begin();
	for(; it2 != buildData.end(); it2++)
		delete it2->second;
	buildData.clear();
	name2index.clear();
	index2data.clear();

    batch = NULL;	
    this->vertexCount = 0;
    this->nextIndex = 0;
}
	






































	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};