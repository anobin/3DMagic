#ifndef MAGIC3D_GEOMETRY_H
#define MAGIC3D_GEOMETRY_H

#include <Math\Math.h>

namespace Magic3D
{
    
class TriangleMesh;
class CollisionShape;
class Sphere;

class Geometry
{
public:
    struct Transform
    {
        Matrix3 rotation;
        Vector3 translation;
        Vector3 scale;

        inline Transform() : scale(1, 1, 1) {}

        void getCombinedMatrix(Matrix4& matrix) const
        {
            Matrix4 tmp;

            matrix.set(Matrix4(rotation));

            tmp.createTranslationMatrix(translation.x(), translation.y(), translation.z());
            matrix.multiply(tmp);

            tmp.createScaleMatrix(scale.x(), scale.y(), scale.z());
            matrix.multiply(tmp);
        }
    };

public:
    virtual void positionTransform(const Transform& transform) = 0;

    virtual const CollisionShape& getCollisionShape() const = 0;

    virtual const TriangleMesh& getTriangleMesh() const = 0;

    // just gets the bounding sphere from the collision shape if not overrided by sub-class
    virtual const Sphere& getBoundingSphere() const;


    void scale(Vector3 factors)
    {
        Transform transform;
        transform.scale = factors;
        this->positionTransform(transform);
    }
    inline void scale(Scalar factor)
    {
        this->scale(Vector3(factor, factor, factor));
    }

    inline void translate(Vector3 distances)
    {
        Transform transform;
        transform.translation = distances;
        this->positionTransform(transform);
    }

    inline void rotate(Scalar angle, Vector3 axis)
    {
        Transform transform;
        transform.rotation.createRotationMatrix(angle * Scalar(M_PI / 180), axis.x(), axis.y(), axis.z());
        this->positionTransform(transform);
    }
    inline void rotate(const Matrix3& matrix)
    {
        Transform transform;
        transform.rotation = matrix;
        this->positionTransform(transform);
    }
};

};


#endif