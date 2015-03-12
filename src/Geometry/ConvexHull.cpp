#include <Geometry\ConvexHull.h>

namespace Magic3D
{

ConvexHull::ConvexHull(const Geometry& exactGeometry, Scalar margin)
{
    // TODO: don't copy mesh data, use direct interface
    // build exact mesh shape, true mesh need not be convex already
    const TriangleMesh& mesh = exactGeometry.getTriangleMesh();
    btTriangleMesh exactMesh;
    for (unsigned int i = 0; i < mesh.getFaceCount(); i++)
    {
        auto face = mesh.getFaceData(i);

        // get three vertices for triangle
        Vector3 a(mesh.getAttributeData(face->indices[0], GpuProgram::AttributeType::VERTEX));
        Vector3 b(mesh.getAttributeData(face->indices[1], GpuProgram::AttributeType::VERTEX));
        Vector3 c(mesh.getAttributeData(face->indices[2], GpuProgram::AttributeType::VERTEX));

        // add triangle to mesh
        exactMesh.addTriangle(btVector3(a.x(), a.y(), a.z()),
            btVector3(b.x(), b.y(), b.z()),
            btVector3(c.x(), c.y(), c.z()), true
            );
    }
    btConvexTriangleMeshShape exactMeshShape(&exactMesh);

    // build simplified hull from exact mesh shape
    this->hull = std::make_shared<btShapeHull>(&exactMeshShape);
    this->hull->buildHull(margin);

    // build convex hull shape from the simplified hull
    this->internalShape = std::make_shared<btConvexHullShape>();
    for (int i = 0; i < this->hull->numIndices(); i++)
    {
        const btVector3& point = this->hull->getVertexPointer()[
            this->hull->getIndexPointer()[i]
        ];
        this->internalShape->addPoint(point);
    }
    this->internalShape->setMargin(margin);
}


};