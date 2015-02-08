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

#include <Graphics/Mesh.h>
#include <CollisionShapes\TriangleMeshCollisionShape.h>
#include <Graphics\MeshBuilder.h>

namespace Magic3D
{
/// destructor
Mesh::~Mesh()
{
	delete[] attributeData;
	delete vertexArray;
}

const VertexArray& Mesh::getVertexArray()
{
	if (this->vertexArray != nullptr)
		return *this->vertexArray;

	this->vertexArray = new VertexArray();
    
    // go through all attributes in batch and copy data into video
    // memory buffer
    for (int i=0; i < this->attributeCount; i++)
    {
        Mesh::AttributeData& d = this->attributeData[i];
        d.buffer.allocate( d.dataLen, d.data, Buffer::STATIC_DRAW ); // TODO: determine if this should be static draw
		this->vertexArray->setAttributeArray(
			(int)d.type,
			GpuProgram::attributeTypeCompCount[(int)d.type],
			VertexArray::FLOAT, // make this dynamic per each attribute
			d.buffer
		);
    }

	return *this->vertexArray;
}
	
	
/*void Batch::applyTransform(const Matrix4& matrix)
{
MeshBuilder bb;
bb.modify(this);
float temp[3];
bb.setCurrentVertex(0);
for(int j=0; j < this->getVertexCount(); j++)
{
bb.getVertex3f( &temp[0], &temp[1], &temp[2] );
Point3 point(temp);
point = point.transform(matrix);
bb.vertex3f(point.x(), point.y(), point.z());
}
bb.end();
}*/
	
	
const SphereCollisionShape& Meshes::getBoundingSphere()
{
    // lazy init
    if (this->boundingSphere == nullptr)
    {
        TriangleMeshCollisionShape realShape(*this);
        this->boundingSphere = realShape.getBoundingSphere();
    }

    return (*this->boundingSphere);
}
	

Mesh& Meshes::getBoundingSphereMesh()
{
    // lazy init
    if (this->boundingSphereMesh == nullptr)
    {
        this->boundingSphereMesh = std::make_shared<Mesh>();
        MeshBuilder mb;

        // TODO: use some scalable detail based on the radius
        // TODO: apply a transform for the offset of the bounding sphere to
        //       every vertex of mesh
        mb.buildSphere(
            this->boundingSphereMesh.get(),
            this->getBoundingSphere().getRadius(),
            20,
            20
        );
    }

    return *this->boundingSphereMesh;
}

	
};











