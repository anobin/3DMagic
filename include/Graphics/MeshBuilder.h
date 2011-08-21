/* 
Copyright (c) 2011 Andrew Keating

This file is part of 3DMagic.

3DMagic is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

3DMagic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with 3DMagic.  If not, see <http://www.gnu.org/licenses/>.

*/
/** Header file for MeshBuilder class
 *
 * @file MeshBuilder.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MESH_BUILDER_H
#define MAGIC3D_MESH_BUILDER_H

#include "../Exceptions/MagicException.h"
#include "../Shaders/Shader.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "../Util/Color.h"
#include "../Math/Math.h"
#include "../Util/magic_throw.h"
#include "../Util/magic_assert.h"

#include <vector>

namespace Magic3D
{

/** Helps to manage vertex attributes when
 * building and drawing objects.
 */
class MeshBuilder
{
private:	
    /// datatype used when building model by hand, before end() call
	struct BuildData
	{
		float* temp;
		int tempLength;
		int currentIndex;
		int currentVertex;
		
		inline BuildData(Mesh::AttributeType type, int vertexCount )
		{
		    // type for auto-bound attribute is always float
		    int components = Mesh::attributeTypeCompCount[(int)type];
		    this->tempLength = vertexCount * components;
		    this->temp = new float[ this->tempLength ];
		    this->currentIndex = 0;
		    this->currentVertex = 0;
		}
		
		inline ~BuildData()
		{
		    delete[] temp;
		}
		    
	};
    
	/** mapping of attribute build index to build data
	 */
	 BuildData* buildData[ Mesh::MAX_ATTRIBUTE_TYPES ];
	
	/// the mesh currently being built
	Mesh* batch;
	
	/// number of verticies being managed
	int vertexCount;
	
	inline BuildData* setupBuildData( Mesh::AttributeType type )
	{
	    BuildData* b = this->buildData[(int)type];
	    if (b != NULL)
	        return b; // already setup
	    
	    this->buildData[(int)type] = new BuildData(type, this->vertexCount);
	    return this->buildData[(int)type];
	}
	
	inline void setAttribute4(BuildData* data, float c1, float c2, float c3, float c4)
	{
	    MAGIC_THROW(data->currentVertex >= this->vertexCount, "Went over specified "
	        "vertex count when building a mesh.");
		data->temp[ data->currentIndex   ] = c1;
		data->temp[ data->currentIndex+1 ] = c2;
		data->temp[ data->currentIndex+2 ] = c3;
		data->temp[ data->currentIndex+3 ] = c4;
		data->currentIndex += 4;
		data->currentVertex++;
	}
	
	inline void setAttribute3(BuildData* data, float c1, float c2, float c3)
	{		
	    MAGIC_THROW(data->currentVertex >= this->vertexCount, "Went over specified "
	        "vertex count when building a mesh.");
		data->temp[ data->currentIndex   ] = c1;
		data->temp[ data->currentIndex+1 ] = c2;
		data->temp[ data->currentIndex+2 ] = c3;
		data->currentIndex += 3;
		data->currentVertex++;
	}
	
	inline void setAttribute2(BuildData* data, float c1, float c2)
	{
	    MAGIC_THROW(data->currentVertex >= this->vertexCount, "Went over specified "
	        "vertex count when building a mesh.");
		data->temp[ data->currentIndex   ] = c1;
		data->temp[ data->currentIndex+1 ] = c2;
		data->currentIndex += 2;
		data->currentVertex++;
	}
	

public:
	/** Standard Constructor
	 */
	MeshBuilder();
	
	/// destructor
	~MeshBuilder();
	
	/** Starts a vertex building sequence
	 * @param vertexCount the number of verticies to be handled
	 */
	void begin(int vertexCount, Mesh* batch);
	
	inline void vertex4f(float v1, float v2, float v3, float v4)
	{
	    this->setAttribute4( this->setupBuildData( Mesh::VERTEX ), 
	        v1, v2, v3, v4 );
	}
	
	inline void vertex3f(float v1, float v2, float v3)
	{
	    this->setAttribute4( this->setupBuildData( Mesh::VERTEX ), 
	        v1, v2, v3, 1.0f );
	}
	
	inline void normal3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Mesh::NORMAL ), 
	        v1, v2, v3);
	}
	
	inline void color3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Mesh::COLOR ), 
	        v1, v2, v3);
	}
	
	inline void color2_3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Mesh::COLOR2 ), 
	        v1, v2, v3);
	}
	
	inline void texCoord0_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Mesh::TEX_COORD_0 ), 
	        v1, v2);
	}
	
	inline void texCoord2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Mesh::TEX_COORD_0 ), 
	        v1, v2);
	}
	
	inline void texCoord1_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Mesh::TEX_COORD_1 ), 
	        v1, v2);
	}
	
	inline void texCoord2_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Mesh::TEX_COORD_2 ), 
	        v1, v2);
	}
	
	inline void texCoord3_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Mesh::TEX_COORD_3 ), 
	        v1, v2);
	}
	
	inline void texCoord4_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Mesh::TEX_COORD_4 ), 
	        v1, v2);
	}
	
	inline void texCoord5_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Mesh::TEX_COORD_5 ), 
	        v1, v2);
	}
	
	inline void texCoord6_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Mesh::TEX_COORD_6 ), 
	        v1, v2);
	}
	
	inline void texCoord7_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Mesh::TEX_COORD_7 ), 
	        v1, v2);
	}
	
	inline void tangent3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Mesh::TANGENT ), 
	        v1, v2, v3);
	}
	
	inline void binormal3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Mesh::BINORMAL ), 
	        v1, v2, v3);
	}
	
	/** end a vertex building sequence
	 */
	void end();
	
	/** Build a box mesh
     * @param width the width of the box
     * @param height the height of the box
     * @param depth the depth of the box
     */
	void buildBox(Mesh* mesh, float width, float height, float depth );
	
	/** Build a 2D circle mesh
     * @param x the x coordinate of the center
     * @param y the y coordinate of the center
     * @param radius the radius of the circle
     * @param precisionAngle the angle between any two points on the edge of the
                            circle, the lower angle, the better looking
     */
	void build2DCircle(Mesh* mesh, int x, int y, int radius, float precisionAngle );
	
	/** Build a flat surface
     * @param width the width of the surface
     * @param height the height of the surface
     * @param slices the number of squares on width
     * @param stacks the number of squares on height
     */
    void buildFlatSurface(Mesh* mesh, float width, float height, int slices, 
        int stacks, bool texRepeat, float texPerX, float texPerY);

    /** Build 2D rectangle
     * @param x the upper-left x coordinate of the rectangle
     * @param y the upper-left y coordinate of the rectangle
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     */
    void build2DRectangle(Mesh* mesh, int x, int y, int width, int height);
    
    /** Build sphere
     * @param radius the radius of the sphere
     * @param slices the number of squares on width
     * @param stacks the number of squares on height
     */
    void buildSphere(Mesh* mesh, float radius, int slices, int stacks);
    
};


};




#endif
















