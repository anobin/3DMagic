#include <Geometry\CompoundGeometry.h>
#include <BulletCollision\CollisionShapes\btCompoundShape.h>
#include <CollisionShapes\CollisionShape.h>
#include <Mesh\TriangleMesh.h>
#include <Geometry\Sphere.h>

namespace Magic3D
{

const CollisionShape& CompoundGeometry::getCollisionShape() const
{
    // TODO: get this to work
    throw_MagicException("Collision Shape Generation from a compound geometry is not currently supported");

    if (this->shape != nullptr)
        return *this->shape;

    auto compound = std::make_shared<btCompoundShape>(false);
    for (auto part : this->list)
    {
        // TODO: extract transform from individual collision shapes
        compound->addChildShape(btTransform(), &part->getCollisionShape()._getShape());
    }

    this->shape = std::make_shared<CollisionShape>(compound);
    return *this->shape;
}

const TriangleMesh& CompoundGeometry::getTriangleMesh() const
{
    if (this->mesh != nullptr)
        return *this->mesh;

    unsigned int vertexCount = 0;
    unsigned int faceCount = 0;
    // TODO: get attribute types from individual meshes
    std::set<GpuProgram::AttributeType> attrs;
    attrs.insert(GpuProgram::AttributeType::VERTEX);
    attrs.insert(GpuProgram::AttributeType::TEX_COORD_0);
    attrs.insert(GpuProgram::AttributeType::NORMAL);
    attrs.insert(GpuProgram::AttributeType::TANGENT);
    for (auto part : this->list)
    {
        vertexCount += part->getTriangleMesh().getVertexCount();
        faceCount += part->getTriangleMesh().getFaceCount();
    }

    this->mesh = std::make_shared<TriangleMesh>(vertexCount, faceCount, attrs);
    unsigned int currentVertex = 0;
    unsigned int currentFace = 0;
    for (auto part : this->list)
    {
        auto& partMesh = part->getTriangleMesh();

        // copy attribute data for all vertices
        for (auto attrType : attrs)
        {
            // copy all vertex data
            this->mesh->setAttributeData(
                currentVertex, 
                attrType,
                partMesh.getAttributeData(0, attrType),
                partMesh.getVertexCount()
            );
        }
        
        // copy face data, accounting for new base vertex
        for (unsigned int i = 0; i < partMesh.getFaceCount(); i++)
        {
            auto face = partMesh.getFace(i);
            face.indices[0] += currentVertex;
            face.indices[1] += currentVertex;
            face.indices[2] += currentVertex;
            this->mesh->setFace(currentFace + i, face);
        }

        currentVertex += partMesh.getVertexCount();
        currentFace += partMesh.getFaceCount();
    }

    return *this->mesh;
}

const Sphere& CompoundGeometry::getBoundingSphere() const
{
    // TODO: calculate from collision shape once that works
    return this->getTriangleMesh().getBoundingSphere();
}

};