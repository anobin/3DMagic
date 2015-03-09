#include <Geometry\BoundedPlane.h>

namespace Magic3D
{

const TriangleMesh& BoundedPlane::getTriangleMesh() const
{
    if (this->triangleMesh != nullptr)
        return *this->triangleMesh;

    std::set<GpuProgram::AttributeType> attrs;
    attrs.insert(GpuProgram::AttributeType::VERTEX);
    attrs.insert(GpuProgram::AttributeType::TEX_COORD_0);
    attrs.insert(GpuProgram::AttributeType::NORMAL);
    attrs.insert(GpuProgram::AttributeType::TANGENT);

    int vertexCount = slices*stacks * 6;
    this->triangleMesh = std::make_shared<TriangleMesh>(vertexCount, vertexCount / 3, attrs);
    auto mesh = this->triangleMesh;
    int currentVertex = 0;

    float x = -width / 2;
    float z = -height / 2;
    float xStep = width / slices;
    float zStep = height / stacks;
    float texX = (width / slices) / texPerX;
    float texY = (height / stacks) / texPerY;

    for (unsigned int i = 0, j = 0;;)
    {
        // top left
        auto vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x, 0.0f, z);
        vert.texCoord(texX*i, texY*j);
        mesh->setVertex(currentVertex, vert);
        currentVertex++;

        // bottom left
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x, 0.0f, z + zStep);
        vert.texCoord(texX*i, texY*(j + 1));
        mesh->setVertex(currentVertex, vert);
        currentVertex++;

        // top right
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x + xStep, 0.0f, z);
        vert.texCoord(texX*(i + 1), texY*j);
        mesh->setVertex(currentVertex, vert);
        currentVertex++;

        // top right
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x + xStep, 0.0f, z);
        vert.texCoord(texX*(i + 1), texY*j);
        mesh->setVertex(currentVertex, vert);
        currentVertex++;

        // bottom left
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x, 0.0f, z + zStep);
        vert.texCoord(texX*i, texY*(j + 1));
        mesh->setVertex(currentVertex, vert);
        currentVertex++;

        // bottom right
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x + xStep, 0.0f, z + zStep);
        vert.texCoord(texX*(i + 1), texY*(j + 1));
        mesh->setVertex(currentVertex, vert);
        currentVertex++;

        i++;
        if (i < slices)
            x += xStep;
        else
        {
            i = 0;
            x = -width / 2;
            j++;
            if (j >= stacks)
                break; // done
            z += zStep;
        }
    }

    for (int i = 0; i < vertexCount; i += 3)
    {
        mesh->setFace(i / 3, TriangleMesh::Face(i, i + 1, i + 2));
    }

    mesh->positionTransform(this->transform);
    mesh->calculateNormalsAndTangents();

    return *mesh;
}


};