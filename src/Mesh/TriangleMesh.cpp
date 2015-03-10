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

    this->collisionMesh = std::make_shared<btTriangleMesh>();
    for (unsigned int i = 0; i < this->getFaceCount(); i++)
    {
        auto face = this->getFaceData(i);

        // get three vertices for triangle
        Vector3 a(this->getAttributeData(face->indices[0], GpuProgram::AttributeType::VERTEX));
        Vector3 b(this->getAttributeData(face->indices[1], GpuProgram::AttributeType::VERTEX));
        Vector3 c(this->getAttributeData(face->indices[2], GpuProgram::AttributeType::VERTEX));

        // add triangle to mesh
        this->collisionMesh->addTriangle(btVector3(a.x(), a.y(), a.z()),
            btVector3(b.x(), b.y(), b.z()),
            btVector3(c.x(), c.y(), c.z()), true
            );
    }

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