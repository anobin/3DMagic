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
VertexHandler::VertexHandler(const ShaderVertexInterfaceSpec& spec): vertexCount(0)
{
	const std::map<int, ShaderVertexInterfaceSpec::AttributeTypes>& m = 
		spec.getAttributeMappings();
	std::map<int, ShaderVertexInterfaceSpec::AttributeTypes>::const_iterator it = 
		m.begin();
	for (; it != m.end(); it++)
	{
		AttributeData* d = new AttributeData;
		d->index = it->first;
		d->buffer = NULL;
		d->temp = NULL;
		data.insert(std::pair<ShaderVertexInterfaceSpec::AttributeTypes, 
					AttributeData*>(it->second, d));
	}
}
			
/** Standard Constructor for specifying multiple shaders
 * that need to have their requirements meet
 * @param specCount the number of specs provided
 * @param ... const references to the shader-vertex interface specs to use
 */
VertexHandler::VertexHandler(int specCount, ...): vertexCount(0)
{
	throw MagicExceptionMacro("Not supported yet");
}
	
/// destructor
VertexHandler::~VertexHandler()
{
	std::map<ShaderVertexInterfaceSpec::AttributeTypes, AttributeData*>::iterator it
		= data.begin();
	for(; it != data.end(); it++)
	{
		delete it->second->buffer;
		delete[] it->second->temp;
		delete it->second;
	}
}
	
/** Starts a vertex building sequence
 * @param vertexCount the number of verticies to be handled
 */
void VertexHandler::begin(int vertexCount)
{
	if (this->vertexCount != 0)
		throw MagicExceptionMacro("Attempt to rebuild vertex handler");
		
	this->vertexCount = vertexCount;
	
	// build temp storage to be used while building
	std::map<ShaderVertexInterfaceSpec::AttributeTypes, AttributeData*>::iterator it =
		data.begin();
	for(; it != data.end(); it++)
	{
		switch(it->first)
		{
			case ShaderVertexInterfaceSpec::POSITION:		it->second->components = 4; break;
			case ShaderVertexInterfaceSpec::NORMAL:			it->second->components = 3; break;
			case ShaderVertexInterfaceSpec::COLOR:			it->second->components = 4; break;
			case ShaderVertexInterfaceSpec::BASE_TEXTURE:	it->second->components = 2; break;
			default:
				throw ShaderVertexInterfaceExceptionMacro("Tried to begin a vertex"
					" build with an invalid attribute type");
		}
		it->second->temp = new GLfloat[vertexCount*it->second->components];
		it->second->tempLength = vertexCount*it->second->components;
		it->second->currentVertex = 0;
	}
}

/** end a vertex building sequence
 */
void VertexHandler::end()
{
	// place all data into graphics memory and bind to vertex array
	std::map<ShaderVertexInterfaceSpec::AttributeTypes, AttributeData*>::iterator it =
		data.begin();
	for(; it != data.end(); it++)
	{
		it->second->buffer = new Buffer(it->second->tempLength*sizeof(GLfloat),
										it->second->temp,
										Buffer::STATIC_DRAW
										);

		// delete temp data
		delete[] it->second->temp;
		it->second->temp = NULL;
		it->second->tempLength = 0;
		
		// bind buffer to attribute index
#ifndef MAGIC3D_NO_VERTEX_ARRAYS
		array.setAttributeArray(it->second->index, it->second->components,
								VertexArray::FLOAT, *it->second->buffer);
#endif
	}
	
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};