#include <Mesh\TriangleMesh.h>

namespace Magic3D
{

void TriangleMesh::positionTransform(const Matrix4& matrix)
{
    for (unsigned int i = 0; i < this->vertexCount; i++)
    {
        auto vert = this->getVertex<PositionAttr>(i);
        vert.position(Vector3(vert.position()).transform(matrix));
        this->setVertex(i, vert);
    }
}

const CollisionShape& TriangleMesh::getCollisionShape() const
{
    if (this->collisionShape != nullptr)
        return *this->collisionShape;

    this->collisionMesh = std::make_shared<btTriangleIndexVertexArray>();
    btIndexedMesh mesh;

    mesh.m_numTriangles = this->faces.size();
    mesh.m_triangleIndexBase = (const unsigned char*)&this->faces[0];
    mesh.m_triangleIndexStride = 3 * sizeof(unsigned int);
    mesh.m_indexType = PHY_ScalarType::PHY_INTEGER; // actually means unsigned int

    mesh.m_numVertices = this->vertexCount;
    mesh.m_vertexBase = (const unsigned char*)this->getAttributeData(0, GpuProgram::AttributeType::VERTEX);
    mesh.m_vertexStride = 4 * sizeof(Scalar);
    mesh.m_vertexType = PHY_ScalarType::PHY_FLOAT;

    this->collisionMesh->addIndexedMesh(mesh, PHY_ScalarType::PHY_INTEGER);

    this->collisionShape = std::make_shared<CollisionShape>(
        std::make_shared<btBvhTriangleMeshShape>(this->collisionMesh.get(), true)
    );
    return *this->collisionShape;
}

const TriangleMesh& TriangleMesh::getTriangleMesh() const
{
    return *this;
}

};