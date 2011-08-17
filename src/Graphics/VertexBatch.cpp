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
/** Implementation file for VertexBatch class
 * 
 * @file VertexHandler.cpp
 * @author Andrew Keating
 */

#include <Graphics/VertexBatch.h>

namespace Magic3D
{
	
/// destructor
VertexBatch::~VertexBatch()
{
	std::vector< AttributeData* >::iterator it2 =  attributeData.begin();
	for(; it2 != attributeData.end(); it2++)
	    delete (*it2);
}
	

template<> 
float VertexBatch::getProperty<float>(PropertyIndex p)
{
    return properties[(int)p].data.u_float;
}


template<>
Texture* VertexBatch::getProperty<Texture*>(PropertyIndex p)
{
    return properties[(int)p].data.u_texture;
}

template<>
void VertexBatch::setProperty<float>(PropertyIndex p, float f)
{
    properties[(int)p].data.u_float = f;
    properties[(int)p].set = true;
}

template<>
void VertexBatch::setProperty<Texture*>(PropertyIndex p, Texture* t)
{
    properties[(int)p].data.u_texture = t;
    properties[(int)p].set = true;
}

	
	
	
	
	
	
	
	
	
	
	
};