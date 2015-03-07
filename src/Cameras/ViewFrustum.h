#ifndef MAGIC3D_VIEW_FRUSTUM_H
#define MAGIC3D_VIEW_FRUSTUM_H

#include <Math\Matrix4.h>
#include <memory>


namespace Magic3D
{ 

class _Plane
{
    Vector3 normal;
    float d;

public:
    inline _Plane() {}

    inline _Plane(Vector3& topRight, Vector3& topLeft, Vector3& bottomLeft)
    {
        this->update(topRight, topLeft, bottomLeft);
    }

    void update(Vector3& topRight, Vector3& topLeft, Vector3& bottomLeft) 
    {
        Vector3 u, v;

        u = topRight - topLeft;
        v = bottomLeft - topLeft;

        normal = (v * u).normalize();

        d = -(normal.dotProduct(topLeft));
    }

    float distance(Vector3 &p) 
    {
        return (d + normal.dotProduct(p));
    }
};

class Rectangle
{
public:
    Vector3 topLeft;
    Vector3 topRight;
    Vector3 bottomLeft;
    Vector3 bottomRight;

    inline Rectangle() {}

    inline Rectangle(Vector3 topLeft, Vector3 topRight, Vector3 bottomLeft,
        Vector3 bottomRight) : topLeft(topLeft), topRight(topRight),
        bottomLeft(bottomLeft), bottomRight(bottomRight) {}

    inline void update(Vector3 topLeft, Vector3 topRight, Vector3 bottomLeft,
        Vector3 bottomRight)
    {
        this->topLeft = topLeft;
        this->topRight = topRight;
        this->bottomLeft = bottomLeft;
        this->bottomRight = bottomRight;
    }

    inline Scalar getHeight()
    {
        return topLeft.distanceTo(bottomLeft);
    }

    inline Scalar getWidth()
    {
        return topLeft.distanceTo(topRight);
    }
};

class ViewFrustum
{
    _Plane pl[6];

    Rectangle nearRec;
    Rectangle farRec;
    Scalar zNear, zFar, ratio, angle;
    Scalar nw, nh, fw, fh;

    enum {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGHT,
        NEARP,
        FARP
    };

public:
    void setCamProperties(float angle, float ratio, float zNear, float zFar) 
    {
        this->ratio = ratio;
        this->angle = angle;
        this->zNear = zNear;
        this->zFar = zFar;

        Scalar tang = (float)tan(angle* (M_PI/180.0f) * 0.5);
        nh = zNear * tang;
        nw = nh * ratio;
        fh = zFar  * tang;
        fw = fh * ratio;
    }

    void setPosition(const Position& pos) 
    {
        Vector3 p(pos.getLocation().x(), pos.getLocation().y(), pos.getLocation().z());
        Vector3 nc, fc, X, Y, Z;

        Z = (pos.getForwardVector() * -1).normalize();

        X = (pos.getUpVector() * Z).normalize();

        Y = Z * X;

        nc = p - Z * zNear;
        fc = p - Z * zFar;

        nearRec.update(
            nc + Y * nh - X * nw, // top left
            nc + Y * nh + X * nw, // top right
            nc - Y * nh - X * nw, // bottom left
            nc - Y * nh + X * nw  // bottom right
        );

        farRec.update(
            fc + Y * fh - X * fw,
            fc + Y * fh + X * fw,
            fc - Y * fh - X * fw,
            fc - Y * fh + X * fw
        );

        pl[TOP].update(nearRec.topRight, nearRec.topLeft, farRec.topLeft);
        pl[BOTTOM].update(nearRec.bottomLeft, nearRec.bottomRight, farRec.bottomRight);
        pl[LEFT].update(nearRec.topLeft, nearRec.bottomLeft, farRec.bottomLeft);
        pl[RIGHT].update(nearRec.bottomRight, nearRec.topRight, farRec.bottomRight);
        pl[NEARP].update(nearRec.topLeft, nearRec.topRight, nearRec.bottomRight);
        pl[FARP].update(farRec.topRight, farRec.topLeft, farRec.bottomLeft);
    }

    bool sphereInFrustum(Vector3 &p, float raio) 
    {
        float distance;

        for (int i = 0; i < 6; i++) 
        {
            distance = pl[i].distance(p);
            if (distance < -raio)
                return false;
        }
        return true;
    }
};


};


#endif