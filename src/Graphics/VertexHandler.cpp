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
/** Implementation file for VertexHandler class
 * 
 * @file VertexHandler.cpp
 * @author Andrew Keating
 */

#include <Graphics/VertexHandler.h>

namespace Magic3D
{
	
/** Standard Constructor
 * @param spec the shader-vertex interface spec to use
 */
VertexHandler::VertexHandler(const VertexAttribSpec* spec): vertexCount(0), spec(spec)
{
    for( int i = 0; i < (int) VertexAttribSpec::BUILTIN_ATTRIB_COUNT; i++ )
        builtin_index[ i ] = -1;
}
	
/// destructor
VertexHandler::~VertexHandler()
{
	std::map<int, AttributeData*>::iterator it
		= data.begin();
	for(; it != data.end(); it++)
		delete it->second;
}
	
/** Starts a vertex building sequence
 * @param vertexCount the number of verticies to be handled
 */
void VertexHandler::begin(int vertexCount)
{
	if (this->vertexCount != 0)
		throw_MagicException("Attempt to rebuild vertex handler");
		
	this->vertexCount = vertexCount;
}

/** end a vertex building sequence
 */
void VertexHandler::end()
{
	// place all data into graphics memory and bind to vertex array
	std::map<int, AttributeData*>::iterator it =
		data.begin();
	for(; it != data.end(); it++)
	{
		it->second->buffer = new Buffer(it->second->tempLength*
		    VertexArray::getDataTypeSize(it->second->type), it->second->temp,
			Buffer::STATIC_DRAW );

		// delete temp data
		delete[] it->second->temp;
		it->second->temp = NULL;
		it->second->tempLength = 0;
		
		// bind buffer to attribute index
#ifndef MAGIC3D_NO_VERTEX_ARRAYS
		array.setAttributeArray(it->second->index, it->second->components,
								it->second->type, *it->second->buffer);
#endif
	}
	
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};