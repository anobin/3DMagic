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
	delete vertexArray;
}

const VertexArray& Mesh::getVertexArray()
{
	if (this->vertexArray != nullptr)
		return *this->vertexArray;

	this->vertexArray = new VertexArray();
    
    // go through all attributes in batch and copy data into video
    // memory buffer
    for (int i=0; i < this->attributeData.size(); i++)
    {
        Mesh::AttributeData& d = this->attributeData[i];
        d.buffer.allocate( d.data.size() * sizeof(float), &d.data[0], Buffer::STATIC_DRAW ); // TODO: determine if this should be static draw
		this->vertexArray->setAttributeArray(
			(int)d.type,
			GpuProgram::attributeTypeCompCount[(int)d.type],
			VertexArray::FLOAT, // make this dynamic per each attribute
			d.buffer
		);
    }

	return *this->vertexArray;
}
	
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

        // TODO: use some scalable detail based on the radius
        // TODO: apply a transform for the offset of the bounding sphere to
        //       every vertex of mesh
        MeshBuilderPTNT mb;
        mb.buildSphere(
            this->getBoundingSphere().getRadius(),
            20,
            20
        );

        auto sphere = this->getBoundingSphere();
        Matrix4 moveMatrix;
        moveMatrix.createTranslationMatrix(
            sphere.getOffset().x(),
            sphere.getOffset().y(),
            sphere.getOffset().z()
        );
        mb.positionTransform(moveMatrix);

        this->boundingSphereMesh = mb.build();
    }

    return *this->boundingSphereMesh;
}

Mesh& Mesh::getVisibleNormals()
{
    if (this->visibleNormals == nullptr)
    {
        MeshBuilder<PositionAttr> mb(this->vertexCount*6, VertexArray::Primitives::LINES);
        for (int i = 0; i < this->vertexCount; i++)
        {
            auto vertex = this->getVertex(i);

            mb.addVertex(vertex.position());
            mb.addVertex(Vector3(
                vertex.position().x() + vertex.normal().x(),
                vertex.position().y() + vertex.normal().y(),
                vertex.position().z() + vertex.normal().z()
            ));

            mb.addVertex(vertex.position());
            mb.addVertex(Vector3(
                vertex.position().x() + vertex.tangent().x(),
                vertex.position().y() + vertex.tangent().y(),
                vertex.position().z() + vertex.tangent().z()
            ));

            Vector3 biTangent = vertex.normal() * vertex.tangent();

            mb.addVertex(vertex.position());
            mb.addVertex(Vector3(
                vertex.position().x() + biTangent.x(),
                vertex.position().y() + biTangent.y(),
                vertex.position().z() + biTangent.z()
            ));
        }

        this->visibleNormals = mb.build();
    }

    return *this->visibleNormals;
}


std::shared_ptr<Mesh> Mesh::applyTransform(const Matrix4& matrix) const
{
    MeshBuilderPTNT mb(this->vertexCount, this->primitive);
    for (int j = 0; j < this->getVertexCount(); j++)
    {
        auto vertex = this->getVertex(j);
        vertex.position(Vector3(vertex.position().getData()).transform(matrix));
        mb.addVertex(vertex);
    }
    return mb.build();
}

	
};











