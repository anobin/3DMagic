#ifndef MAGIC3D_VERTEX_H
#define MAGIC3D_VERTEX_H

#include <tuple>

#include <Math/Vector.h>


namespace Magic3D
{

class PositionAttr
{
    Scalar* data;
public:

    static const GpuProgram::AttributeType type = GpuProgram::AttributeType::VERTEX;

    inline PositionAttr(Scalar* data) : data(data) {}

    inline void position(Scalar x, Scalar y, Scalar z)
    {
        this->position(x, y, z, 1.0f);
    }

    inline void position(Scalar x, Scalar y, Scalar z, Scalar w)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }

    inline void position(const Vector4& vec)
    {
        this->position(vec.x(), vec.y(), vec.z(), vec.z());
    }

    inline void position(const Vector3& vec)
    {
        this->position(vec.x(), vec.y(), vec.z());
    }

    inline Vector4 position() const
    {
        return this->data;
    }

    inline const Scalar* getData() const
    {
        return this->data;
    }
};


class TexCoordAttr
{
    Scalar* data;

public:
    static const GpuProgram::AttributeType type = GpuProgram::AttributeType::TEX_COORD_0;

    inline TexCoordAttr(Scalar* data): data(data) {}

    inline void texCoord(Scalar x, Scalar y)
    {
        data[0] = x;
        data[1] = y;
    }

    inline void texCoord(const Vector2& vec)
    {
        this->texCoord(vec.x(), vec.y());
    }

    inline Vector2 texCoord() const
    {
        return Vector2(data);
    }

    inline const Scalar* getData() const
    {
        return this->data;
    }
};


class NormalAttr
{
    Scalar* data;
public:

    static const GpuProgram::AttributeType type = GpuProgram::AttributeType::NORMAL;

    inline NormalAttr(Scalar* data): data(data) {}

    inline void normal(Scalar x, Scalar y, Scalar z)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }

    inline void normal(const Vector3& vec)
    {
        this->normal(vec.x(), vec.y(), vec.z());
    }

    inline Vector3 normal() const
    {
        return Vector3(data);
    }

    inline const Scalar* getData() const
    {
        return this->data;
    }
};


class TangentAttr
{
    Scalar* data;
public:

    static const GpuProgram::AttributeType type = GpuProgram::AttributeType::TANGENT;

    inline TangentAttr(Scalar* data): data(data) {}

    inline void tangent(Scalar x, Scalar y, Scalar z)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }

    inline void tangent(const Vector3& vec)
    {
        this->tangent(vec.x(), vec.y(), vec.z());
    }

    inline Vector3 tangent() const
    {
        return Vector3(data);
    }

    inline const Scalar* getData() const
    {
        return this->data;
    }
};


template<typename... AttributeTypes> class Vertex {};

template <typename AttrType, typename... AttributeTypes>
class Vertex<AttrType, AttributeTypes...> : 
    public AttrType, public Vertex<AttributeTypes...>
{

public:
    inline Vertex() {}

    inline Vertex(const AttrType& attr, const AttributeTypes&... attrTypes) :
        AttrType(attr), Vertex<AttributeTypes...>(attrTypes...)
    {}

    template<typename VectorType, typename... VectorTypes>
    inline Vertex(VectorType vector, VectorTypes... vectors) :
        AttrType(vector), Vertex<AttributeTypes...>(vectors...)
    {}

    static const int attributeCount = sizeof...(AttributeTypes) + 1;
};


};





#endif