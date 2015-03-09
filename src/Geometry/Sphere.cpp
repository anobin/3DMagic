#include <Geometry\Sphere.h>


namespace Magic3D
{

unsigned int getMiddlePoint(std::vector<Vector3>& points,
    unsigned int p1, unsigned int p2, 
    std::unordered_map<unsigned long long, unsigned int>& pointCache
    )
{
    // first check if we have it already
    bool firstIsSmaller = p1 < p2;
    unsigned long long smallerIndex = firstIsSmaller ? p1 : p2;
    unsigned long long greaterIndex = firstIsSmaller ? p2 : p1;
    unsigned long long key = (smallerIndex << 32) + greaterIndex;

    auto it = pointCache.find(key);
    if (it != pointCache.end())
        return it->second;

    // not in cache, calculate it
    auto point1 = points[p1];
    auto point2 = points[p2];

    // normalize to ensure the point is on the unit sphere
    points.push_back(Vector3(
        (point1.x() + point2.x()) / 2.0f,
        (point1.y() + point2.y()) / 2.0f,
        (point1.z() + point2.z()) / 2.0f
    ).normalize());

    // store it, return index
    pointCache[key] = points.size() - 1;

    return points.size() - 1;
}

const TriangleMesh& Sphere::getTriangleMesh() const
{
    if (this->triangleMesh != nullptr)
        return *this->triangleMesh;

    std::vector<Vector3> points;
    std::vector<TriangleMesh::Face> faces;

    // create 12 vertices of a icosahedron
    Scalar t = (1.0f + sqrt(5.0f)) / 2.0f;

    points.push_back(Vector3(-1, t, 0).normalize());
    points.push_back(Vector3(1, t, 0).normalize());
    points.push_back(Vector3(-1, -t, 0).normalize());
    points.push_back(Vector3(1, -t, 0).normalize());

    points.push_back(Vector3(0, -1, t).normalize());
    points.push_back(Vector3(0, 1, t).normalize());
    points.push_back(Vector3(0, -1, -t).normalize());
    points.push_back(Vector3(0, 1, -t).normalize());

    points.push_back(Vector3(t, 0, -1).normalize());
    points.push_back(Vector3(t, 0, 1).normalize());
    points.push_back(Vector3(-t, 0, -1).normalize());
    points.push_back(Vector3(-t, 0, 1).normalize());

    // 5 faces around point 0
    faces.push_back(TriangleMesh::Face(0, 11, 5));
    faces.push_back(TriangleMesh::Face(0, 5, 1));
    faces.push_back(TriangleMesh::Face(0, 1, 7));
    faces.push_back(TriangleMesh::Face(0, 7, 10));
    faces.push_back(TriangleMesh::Face(0, 10, 11));

    // 5 adjacent faces 
    faces.push_back(TriangleMesh::Face(1, 5, 9));
    faces.push_back(TriangleMesh::Face(5, 11, 4));
    faces.push_back(TriangleMesh::Face(11, 10, 2));
    faces.push_back(TriangleMesh::Face(10, 7, 6));
    faces.push_back(TriangleMesh::Face(7, 1, 8));

    // 5 faces around point 3
    faces.push_back(TriangleMesh::Face(3, 9, 4));
    faces.push_back(TriangleMesh::Face(3, 4, 2));
    faces.push_back(TriangleMesh::Face(3, 2, 6));
    faces.push_back(TriangleMesh::Face(3, 6, 8));
    faces.push_back(TriangleMesh::Face(3, 8, 9));

    // 5 adjacent faces 
    faces.push_back(TriangleMesh::Face(4, 9, 5));
    faces.push_back(TriangleMesh::Face(2, 4, 11));
    faces.push_back(TriangleMesh::Face(6, 2, 10));
    faces.push_back(TriangleMesh::Face(8, 6, 7));
    faces.push_back(TriangleMesh::Face(9, 8, 1));

    // refine triangles
    std::unordered_map<unsigned long long, unsigned int> pointCache;
    for (int i = 0; i < this->fidelity-1; i++)
    {
        std::vector<TriangleMesh::Face> newFaces;
        for (auto face : faces)
        {
            // replace triangle by 4 triangles
            unsigned int a = getMiddlePoint(points, face.indices[0], face.indices[1], 
                pointCache);
            unsigned int b = getMiddlePoint(points, face.indices[1], face.indices[2],
                pointCache);
            unsigned int c = getMiddlePoint(points, face.indices[2], face.indices[0],
                pointCache);

            newFaces.push_back(TriangleMesh::Face(face.indices[0], a, c));
            newFaces.push_back(TriangleMesh::Face(face.indices[1], b, a));
            newFaces.push_back(TriangleMesh::Face(face.indices[2], c, b));
            newFaces.push_back(TriangleMesh::Face(a, b, c));
        }
        std::swap(faces, newFaces);
    }


    std::set<GpuProgram::AttributeType> attrs;
    attrs.insert(GpuProgram::AttributeType::VERTEX);
    attrs.insert(GpuProgram::AttributeType::TEX_COORD_0);
    attrs.insert(GpuProgram::AttributeType::NORMAL);
    attrs.insert(GpuProgram::AttributeType::TANGENT);

    this->triangleMesh = std::make_shared<TriangleMesh>(
        points.size(), faces.size(), attrs);

    for (unsigned int i = 0; i < points.size(); i++)
        this->triangleMesh->setVertex(i, makeVertex<PositionAttr>(points[i]));
    for (unsigned int i = 0; i < faces.size(); i++)
        this->triangleMesh->setFace(i, faces[i]);

    Matrix4 matrix;
    matrix.createScaleMatrix(this->radius, this->radius, this->radius);
    matrix.multiply(this->transform);
    triangleMesh->positionTransform(matrix);

    triangleMesh->calculateNormalsAndTangents();

    return *triangleMesh;
}


};