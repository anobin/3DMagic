#ifndef MAGIC3D_TRIANGLE_H
#define MAGIC3D_TRIANGLE_H

#include <Math/Vector.h>


namespace Magic3D
{ 



class Triangle
{
    Vector3 points[3];

    Vector3 _normal;

    inline void calcNormal()
    {
        this->_normal = (b - a) * (c - a);
    }

public:
    const Vector3& a;
    const Vector3& b;
    const Vector3& c;
    const Vector3& normal;

    inline Triangle(Vector3 a, Vector3 b, Vector3 c) : 
        a(this->points[0]), b(this->points[1]), c(this->points[2]), normal(_normal)
    {
        points[0] = a;
        points[1] = b;
        points[2] = c;
        calcNormal();
    }

    inline Triangle(Vector3 points[3]) : 
        a(this->points[0]), b(this->points[1]), c(this->points[2]), normal(_normal)
    {
        this->points[0] = points[0];
        this->points[1] = points[1];
        this->points[2] = points[2];
        calcNormal();
    }

    inline Triangle(const Triangle& tri) : 
        a(this->points[0]), b(this->points[1]), c(this->points[2]), normal(_normal)
    {
        this->points[0] = tri.points[0];
        this->points[1] = tri.points[1];
        this->points[2] = tri.points[2];
        this->_normal = tri.normal;
    }

    inline const Vector3& getPoint(int index)
    {
        return this->points[index];
    }
};


};





#endif