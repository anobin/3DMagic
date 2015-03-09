#include <Mesh\TriangleMesh.h>
#include <CollisionShapes\TriangleMeshCollisionShape.h>

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
    if (this->collisionShape == nullptr)
        this->collisionShape = std::make_shared<TriangleMeshCollisionShape>(*this);
    return *this->collisionShape;
}

const TriangleMesh& TriangleMesh::getTriangleMesh() const
{
    return *this;
}

};