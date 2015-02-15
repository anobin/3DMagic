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
	
	
/*void Mesh::applyTransform(const Matrix4& matrix)
{
    delete this->vertexArray;
    this->vertexArray = nullptr;

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
	
	
/*const SphereCollisionShape& Meshes::getBoundingSphere()
{
    // lazy init
    if (this->boundingSphere == nullptr)
    {
        TriangleMeshCollisionShape realShape(*this);
        this->boundingSphere = realShape.getBoundingSphere();
    }

    return (*this->boundingSphere);
}*/
	

/*Mesh& Meshes::getBoundingSphereMesh()
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

        auto sphere = this->getBoundingSphere();
        Matrix4 moveMatrix;
        moveMatrix.createTranslationMatrix(
            sphere.getOffset().x(),
            sphere.getOffset().y(),
            sphere.getOffset().z()
        );
        this->boundingSphereMesh->applyTransform(moveMatrix);
    }

    return *this->boundingSphereMesh;
}*/

/*Mesh& Mesh::getVisibleNormals()
{
    if (this->visibleNormals == nullptr)
    {
        this->visibleNormals = std::make_shared<Mesh>();

        MeshBuilder mb;
        mb.begin(this->vertexCount * 2, 1, this->visibleNormals.get(),
            VertexArray::Primitives::LINES);

        MeshBuilder mr;
        mr.modify(this);

        for (int i = 0; i < this->vertexCount; i++)
        {
            Scalar x, y, z;
            Scalar nx, ny, nz;

            mr.setCurrentVertex(i);
            mr.getVertex3f(&x, &y, &z);
            mr.getNormal3f(&nx, &ny, &nz);

            mb.vertex3f(x, y, z);
            mb.vertex3f(x+nx, y+ny, z+nz);
        }

        mr.end();

        mb.end();
    }

    return *this->visibleNormals;
}*/

	
};











