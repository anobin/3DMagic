#ifndef MAGIC3D_GEOMETRY_H
#define MAGIC3D_GEOMETRY_H

#include <Math\Math.h>

namespace Magic3D
{
    
class TriangleMesh;
class CollisionShape;

class Geometry
{
public:
    virtual void positionTransform(const Matrix4& matrix) = 0;

    virtual const CollisionShape& getCollisionShape() const = 0;

    virtual const TriangleMesh& getTriangleMesh() const = 0;



    void scale(Vector3 factors)
    {
        Matrix4 matrix; matrix.createScaleMatrix(factors.x(), factors.y(), factors.z());
        this->positionTransform(matrix);
    }
    inline void scale(Scalar factor)
    {
        Matrix4 matrix; matrix.createScaleMatrix(factor, factor, factor);
        this->positionTransform(matrix);
    }

    inline void translate(Vector3 distances)
    {
        Matrix4 matrix; matrix.createTranslationMatrix(distances.x(), distances.y(), distances.z());
        this->positionTransform(matrix);
    }

    inline void rotate(Scalar angle, Vector3 axis)
    {
        Matrix4 matrix; matrix.createRotationMatrix(angle * Scalar(M_PI / 180), axis.x(), axis.y(), axis.z());
        this->positionTransform(matrix);
    }
};

};


#endif