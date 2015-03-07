#include <Mesh\TriangleMesh.h>
#include <CollisionShapes\TriangleMeshCollisionShape.h>

namespace Magic3D
{

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