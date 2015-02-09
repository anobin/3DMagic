#ifndef MAGIC3D_VIEW_FRUSTUM_H
#define MAGIC3D_VIEW_FRUSTUM_H

#include <Math\Point.h>
#include <Math\Matrix4.h>
#include <memory>
#include <Graphics\Mesh.h>
#include <Graphics\MeshBuilder.h>

namespace Magic3D
{ 

class ViewPlane
{
public:
    Point3 topLeft;
    Point3 topRight;
    Point3 bottomLeft;
    Point3 bottomRight;
};


class ViewFrustum
{
    enum {
        NEARP = 0, FARP, TOP, LEFT, RIGHT, BOTTOM
    };

    ViewPlane planes[6];

    Matrix4 projectionMatrix;

    Scalar fov;
    Scalar aspectRatio;
    Scalar zMin;
    Scalar zMax;

public:
    // plane normals of planes need to point inward
    ViewFrustum(Scalar fov, Scalar aspectRatio, Scalar zMin, Scalar zMax)
    {
        this->fov = fov;
        this->aspectRatio = aspectRatio;
        this->zMin = zMin;
        this->zMax = zMax;

        projectionMatrix.createPerspectiveMatrix(fov, aspectRatio, zMin, zMax);

        Scalar fovRads = fov * M_PI / 360;

        Scalar yMax = Scalar(zMin * tan(fovRads));
        Scalar yMin = -yMax;
        Scalar xMin = yMin * aspectRatio;
        Scalar xMax = -xMin;

        planes[NEARP].topLeft = Point3(xMax, yMax, zMin);
        planes[NEARP].topRight = Point3(xMin, yMax, zMin);
        planes[NEARP].bottomLeft = Point3(xMax, yMin, zMin);
        planes[NEARP].bottomRight = Point3(xMin, yMin, zMin);

        yMax = Scalar(zMax * tan(fovRads));
        yMin = -yMax;
        xMin = yMin * aspectRatio;
        xMax = -xMin;

        planes[FARP].topLeft = Point3(xMin, yMax, zMax);
        planes[FARP].topRight = Point3(xMax, yMax, zMax);
        planes[FARP].bottomLeft = Point3(xMin, yMin, zMax);
        planes[FARP].bottomRight = Point3(xMax, yMin, zMax);

        planes[TOP].topLeft = planes[NEARP].topRight;
        planes[TOP].topRight = planes[NEARP].topLeft;
        planes[TOP].bottomLeft = planes[FARP].topLeft;
        planes[TOP].bottomRight = planes[FARP].topRight;

        planes[LEFT].topLeft = planes[NEARP].topRight;
        planes[LEFT].topRight = planes[FARP].topLeft;
        planes[LEFT].bottomLeft = planes[NEARP].bottomRight;
        planes[LEFT].bottomRight = planes[FARP].bottomLeft;

        planes[RIGHT].topLeft = planes[FARP].topRight;
        planes[RIGHT].topRight = planes[NEARP].topLeft;
        planes[RIGHT].bottomLeft = planes[FARP].bottomRight;
        planes[RIGHT].bottomRight = planes[NEARP].bottomLeft;

        planes[BOTTOM].topLeft = planes[FARP].bottomLeft;
        planes[BOTTOM].topRight = planes[FARP].bottomRight;
        planes[BOTTOM].bottomLeft = planes[NEARP].bottomRight;
        planes[BOTTOM].bottomRight = planes[NEARP].bottomLeft;
    }

    inline const Matrix4& getProjectionMatrix() const
    {
        return this->projectionMatrix;
    }

    std::shared_ptr<Mesh> createMesh() const
    {
        MeshBuilder mb;

        auto mesh = std::make_shared<Mesh>();

        mb.begin(6*6, 3, mesh.get());

        for (int i = 0; i < 6; i++)
        {
            const ViewPlane& plane = this->planes[i];

            // bottom left
            mb.vertex3f(plane.bottomLeft);
            mb.texCoord2f(0.0f, 1.0f);
            mb.normal3f(0.0f, 1.0f, 0.0f);
            // top right
            mb.vertex3f(plane.topRight);
            mb.texCoord2f(1.0f, 0.0f);
            mb.normal3f(0.0f, 1.0f, 0.0f);
            // top left
            mb.vertex3f(plane.topLeft);
            mb.texCoord2f(0.0f, 0.0f);
            mb.normal3f(0.0f, 1.0f, 0.0f);

            // bottom right
            mb.vertex3f(plane.bottomRight);
            mb.texCoord2f(1.0f, 1.0f);
            mb.normal3f(0.0f, 1.0f, 0.0f);
            // top right
            mb.vertex3f(plane.topRight);
            mb.texCoord2f(1.0f, 0.0f);
            mb.normal3f(0.0f, 1.0f, 0.0f);
            // bottom left
            mb.vertex3f(plane.bottomLeft);
            mb.texCoord2f(0.0f, 1.0f);
            mb.normal3f(0.0f, 1.0f, 0.0f);
        }

        mb.end();

        return mesh;
    }

};


};


#endif