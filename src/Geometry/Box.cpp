#include <Geometry\Box.h>
#include <CollisionShapes\CollisionShape.h>

namespace Magic3D
{


void addVertex(std::shared_ptr<TriangleMesh> mesh, int i, Vector3& pos, Vector2& coords)
{
    auto vert = mesh->getVertex<PositionAttr, TexCoordAttr>(i);
    vert.position(pos);
    vert.texCoord(coords);
    mesh->setVertex(i, vert);
}

const TriangleMesh& Box::getTriangleMesh() const
{
    if (this->triangleMesh != nullptr)
        return *triangleMesh;

    // we always work with half lengths
    Scalar width = this->dimensions.x() / 2;
    Scalar height = this->dimensions.y() / 2;
    Scalar depth = this->dimensions.z() / 2;

    std::set<GpuProgram::AttributeType> attrs;
    attrs.insert(GpuProgram::AttributeType::VERTEX);
    attrs.insert(GpuProgram::AttributeType::TEX_COORD_0);
    attrs.insert(GpuProgram::AttributeType::NORMAL);
    attrs.insert(GpuProgram::AttributeType::TANGENT);

    this->triangleMesh = std::make_shared<TriangleMesh>(36, 12, attrs);

    enum {
        BACK_LEFT = 0,
        BACK_RIGHT,
        TOP_LEFT,
        TOP_RIGHT,
        LEFT_TOP,
        MID_TOP_LEFT,
        MID_TOP_RIGHT,
        RIGHT_TOP,
        LEFT_BOT,
        MID_BOT_LEFT,
        MID_BOT_RIGHT,
        RIGHT_BOT,
        BOT_LEFT,
        BOT_RIGHT
    };

    Scalar third = 1.0f / 3.0f;
    Scalar twoThirds = 2.0f / 3.0f;

    // unique points
    Vector3 points[14];
    Vector2 texCoords[14];

    points[BACK_LEFT].set(-width, -height, -depth);
    texCoords[BACK_LEFT].set(third, 1);

    points[BACK_RIGHT].set(width, -height, -depth);
    texCoords[BACK_RIGHT].set(twoThirds, 1);

    points[TOP_LEFT].set(-width, height, -depth);
    texCoords[TOP_LEFT].set(third, 0.75f);

    points[TOP_RIGHT].set(width, height, -depth);
    texCoords[TOP_RIGHT].set(twoThirds, 0.75f);

    points[LEFT_TOP].set(-width, height, -depth);
    texCoords[LEFT_TOP].set(0, 0.5f);

    points[MID_TOP_LEFT].set(-width, height, depth);
    texCoords[MID_TOP_LEFT].set(third, 0.5f);

    points[MID_TOP_RIGHT].set(width, height, depth);
    texCoords[MID_TOP_RIGHT].set(twoThirds, 0.5f);

    points[RIGHT_TOP].set(width, height, -depth);
    texCoords[RIGHT_TOP].set(1, 0.5f);

    points[LEFT_BOT].set(-width, -height, -depth);
    texCoords[LEFT_BOT].set(0, 0.25f);

    points[MID_BOT_LEFT].set(-width, -height, depth);
    texCoords[MID_BOT_LEFT].set(third, 0.25f);

    points[MID_BOT_RIGHT].set(width, -height, depth);
    texCoords[MID_BOT_RIGHT].set(twoThirds, 0.25f);

    points[RIGHT_BOT].set(width, -height, -depth);
    texCoords[RIGHT_BOT].set(1, 0.25f);

    points[BOT_LEFT].set(-width, -height, -depth);
    texCoords[BOT_LEFT].set(third, 0);

    points[BOT_RIGHT].set(width, -height, -depth);
    texCoords[BOT_RIGHT].set(twoThirds, 0);


    // 6 sides, 2 triangles per each
    // duplicate all points as we want unmerged normals for proper shading

    int i = 0;

    // top
    addVertex(triangleMesh, i++, points[TOP_LEFT], texCoords[TOP_LEFT]);
    addVertex(triangleMesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);
    addVertex(triangleMesh, i++, points[TOP_RIGHT], texCoords[TOP_RIGHT]);
    addVertex(triangleMesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);
    addVertex(triangleMesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(triangleMesh, i++, points[TOP_RIGHT], texCoords[TOP_RIGHT]);

    // bottom
    addVertex(triangleMesh, i++, points[BOT_LEFT], texCoords[BOT_LEFT]);
    addVertex(triangleMesh, i++, points[BOT_RIGHT], texCoords[BOT_RIGHT]);
    addVertex(triangleMesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(triangleMesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(triangleMesh, i++, points[BOT_RIGHT], texCoords[BOT_RIGHT]);
    addVertex(triangleMesh, i++, points[MID_BOT_RIGHT], texCoords[MID_BOT_RIGHT]);

    // left side
    addVertex(triangleMesh, i++, points[LEFT_TOP], texCoords[LEFT_TOP]);
    addVertex(triangleMesh, i++, points[LEFT_BOT], texCoords[LEFT_BOT]);
    addVertex(triangleMesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);
    addVertex(triangleMesh, i++, points[LEFT_BOT], texCoords[LEFT_BOT]);
    addVertex(triangleMesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(triangleMesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);

    // right side
    addVertex(triangleMesh, i++, points[RIGHT_TOP], texCoords[RIGHT_TOP]);
    addVertex(triangleMesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(triangleMesh, i++, points[RIGHT_BOT], texCoords[RIGHT_BOT]);
    addVertex(triangleMesh, i++, points[RIGHT_BOT], texCoords[RIGHT_BOT]);
    addVertex(triangleMesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(triangleMesh, i++, points[MID_BOT_RIGHT], texCoords[MID_BOT_RIGHT]);

    // front
    addVertex(triangleMesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);
    addVertex(triangleMesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(triangleMesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(triangleMesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(triangleMesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(triangleMesh, i++, points[MID_BOT_RIGHT], texCoords[MID_BOT_RIGHT]);

    // back
    addVertex(triangleMesh, i++, points[TOP_LEFT], texCoords[TOP_LEFT]);
    addVertex(triangleMesh, i++, points[TOP_RIGHT], texCoords[TOP_RIGHT]);
    addVertex(triangleMesh, i++, points[BACK_LEFT], texCoords[BACK_LEFT]);
    addVertex(triangleMesh, i++, points[BACK_LEFT], texCoords[BACK_LEFT]);
    addVertex(triangleMesh, i++, points[TOP_RIGHT], texCoords[TOP_RIGHT]);
    addVertex(triangleMesh, i++, points[BACK_RIGHT], texCoords[BACK_RIGHT]);

    //generate faces to match with vertices
    for (int i = 0; i < 36; i += 3)
        triangleMesh->setFace(i / 3, TriangleMesh::Face(i, i + 1, i + 2));

    triangleMesh->positionTransform(this->transform);
    triangleMesh->calculateNormalsAndTangents();

    return *triangleMesh;
}

const CollisionShape& Box::getCollisionShape() const
{
    if (collisionShape != nullptr)
        return *this->collisionShape;

    collisionShape = std::make_shared<CollisionShape>(
        std::make_shared<btBoxShape>(btVector3(dimensions.x() / 2, dimensions.y() / 2, dimensions.z() / 2)),
        this->transform.rotation,
        this->transform.translation
    );
    return *this->collisionShape;
}


};