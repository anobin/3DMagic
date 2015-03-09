
#ifndef MAGIC3D_TRIANGLE_MESH_BUILDER_H
#define MAGIC3D_TRIANGLE_MESH_BUILDER_H


#include <sstream>
#include <iomanip>
#include <functional>

#include "../Exceptions/MagicException.h"
#include <Shaders\GpuProgram.h>
#include "../Util/Color.h"
#include "../Math/Math.h"
#include "../Util/magic_throw.h"
#include "../Util/magic_assert.h"
#include <Mesh\TriangleMesh.h>
#include <Shapes\Vertex.h>
#include <Shapes\Triangle.h>

#include <vector>

namespace Magic3D
{

class TriangleMeshBuilder
{
public:
    /** Build 2D rectangle
    * @param x the upper-left x coordinate of the rectangle
    * @param y the upper-left y coordinate of the rectangle
    * @param width the width of the rectangle
    * @param height the height of the rectangle
    */
    static std::shared_ptr<TriangleMesh> build2DRectangle(int x, int y, int width, int height);
};


};








#endif