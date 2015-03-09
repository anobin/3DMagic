
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
    /** Build a box mesh
    * @param width the width of the box
    * @param height the height of the box
    * @param depth the depth of the box
    */
    static std::shared_ptr<TriangleMesh> buildBox(float width, float height, float depth);

    /** Build a 2D circle mesh
    * @param x the x coordinate of the center
    * @param y the y coordinate of the center
    * @param radius the radius of the circle
    * @param precisionAngle the angle between any two points on the edge of the
    circle, the lower angle, the better looking
    */
    static std::shared_ptr<TriangleMesh> build2DCircle(int x, int y, int radius, float precisionAngle);

    /** Build a flat surface
    * @param width the width of the surface
    * @param height the height of the surface
    * @param slices the number of squares on width
    * @param stacks the number of squares on height
    */
    static std::shared_ptr<TriangleMesh> buildFlatSurface(float width, float height, int slices,
        int stacks, bool texRepeat, float texPerX, float texPerY);

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