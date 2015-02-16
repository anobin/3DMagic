#ifndef MAGIC3D_VERTEX_H
#define MAGIC3D_VERTEX_H

#include <tuple>

#include <Math/Vector.h>


namespace Magic3D
{

class PositionAttr
{
    Vector4 data;
public:

    static const GpuProgram::AttributeType type = GpuProgram::AttributeType::VERTEX;

    inline PositionAttr() {}

    inline PositionAttr(const Scalar* data) : data(data) {}

    inline PositionAttr(const Vector3& vec) : data(vec) {}

    inline PositionAttr(Scalar x, Scalar y, Scalar z) : data(x, y, z, 1.0f) {}

    inline PositionAttr(const Vector4& vec) : data(vec) {}

    inline PositionAttr(Scalar x, Scalar y, Scalar z, Scalar w) : data(x, y, z, w) {}

    inline PositionAttr(const PositionAttr& attr) : data(attr.data) {}

    inline void position(Scalar x, Scalar y, Scalar z)
    {
        this->position(x, y, z, 1.0f);
    }

    inline void position(Scalar x, Scalar y, Scalar z, Scalar w)
    {
        this->data = Vector4(x, y, z, w);
    }

    inline void position(const Vector4& vec)
    {
        this->position(vec.x(), vec.y(), vec.z());
    }

    inline void position(const Vector3& vec)
    {
        this->position(vec.x(), vec.y(), vec.z());
    }

    inline Vector4& position()
    {
        return this->data;
    }

    inline const Vector4& position() const
    {
        return this->data;
    }

    inline const Scalar* getData() const
    {
        return this->data.getData();
    }
};


class TexCoordAttr
{
    Vector2 data;

public:
    static const GpuProgram::AttributeType type = GpuProgram::AttributeType::TEX_COORD_0;

    inline TexCoordAttr() {}

    inline TexCoordAttr(const Vector2& vec) : data(vec) {}

    inline TexCoordAttr(Scalar x, Scalar y) : data(x, y) {}

    inline void texCoord(Scalar x, Scalar y)
    {
        data = Vector2(x, y);
    }

    inline void texCoord(const Vector2& vec)
    {
        data = vec;
    }

    inline Vector2& texCoord()
    {
        return data;
    }

    inline const Vector2& texCoord() const
    {
        return data;
    }

    inline const Scalar* getData() const
    {
        return this->data.getData();
    }
};


class NormalAttr
{
    Vector3 data;
public:

    static const GpuProgram::AttributeType type = GpuProgram::AttributeType::NORMAL;

    inline NormalAttr() {}

    inline NormalAttr(const Vector3& vec) : data(vec) {}

    inline NormalAttr(Scalar x, Scalar y, Scalar z) : data(x, y, z) {}

    inline void normal(Scalar x, Scalar y, Scalar z)
    {
        data = Vector3(x, y, z);
    }

    inline void normal(const Vector3& vec)
    {
        data = vec;
    }

    inline Vector3& normal()
    {
        return data;
    }

    inline const Vector3& normal() const
    {
        return data;
    }

    inline const Scalar* getData() const
    {
        return this->data.getData();
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
    inline Vertex(const VectorType& vector, const VectorTypes&... vectors) :
        AttrType(vector), Vertex<AttributeTypes...>(vectors...)
    {}

    static const int attributeCount = sizeof...(AttributeTypes) + 1;
};

template<typename... AttributeTypes>
Vertex<AttributeTypes...> makeVertex(AttributeTypes... types)
{
    return Vertex<AttributeTypes...>(types...);
}

typedef Vertex<PositionAttr, TexCoordAttr, NormalAttr> VertexPTN;

};





#endif