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
/** Implementation file for Mesh class
 * 
 * @file Mesh.cpp
 * @author Andrew Keating
 */

#include <Graphics/Mesh.h>
#include <Graphics/Batch.h>

namespace Magic3D
{
/// destructor
Mesh::~Mesh()
{
	delete[] attributeData;
}

void Mesh::copyBatchIn(const Batch& batch)
{
    // allocate mesh attributes, also clears any previous data
    this->allocate(batch.vertexCount, batch.attributeCount);
    
    // go through all attributes in batch and copy data into video
    // memory buffer
    for (int i=0; i < this->attributeCount; i++)
    {
        Mesh::AttributeData& d = this->attributeData[i];
        const Batch::AttributeData& b = batch.data[i];
        d._type = b.type;
        d._buffer.allocate( b.dataLen, b.data, Buffer::STATIC_DRAW );
    }
}
	
	
	
	
	
	
	
};











