#ifndef MAGIC3D_VERTEX_H
#define MAGIC3D_VERTEX_H

#include <tuple>

#include <Math/Vector.h>


namespace Magic3D
{

class PositionAttr
{
public:
    Vector4 data;

    static const GpuProgram::AttributeType type = GpuProgram::AttributeType::VERTEX;

    inline PositionAttr() {}

    inline PositionAttr(Vector3 vec) : data(vec) {}

    inline PositionAttr(Scalar x, Scalar y, Scalar z) : data(x, y, z, 1.0f) {}

    inline PositionAttr(Vector4 vec) : data(vec) {}

    inline PositionAttr(Scalar x, Scalar y, Scalar z, Scalar w) : data(x, y, z, w) {}

    inline PositionAttr(const PositionAttr& attr) : data(attr.data) {} 

    inline void position(Scalar x, Scalar y, Scalar z)
    {
        data = Vector4(x, y, z, 1);
    }

    inline void position(Scalar x, Scalar y, Scalar z, Scalar w)
    {
        data = Vector4(x, y, z, w);
    }

    inline void position(const Vector4& vec)
    {
        data = vec;
    }

    inline Vector4& position()
    {
        return data;
    }
};


class TexCoordAttr
{
public:
    Vector2 data;

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
};


class NormalAttr
{
public:
    Vector3 data;

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

    inline const Vector3& normal()
    {
        return data;
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