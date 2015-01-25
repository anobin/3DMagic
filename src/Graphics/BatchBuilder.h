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
/** Header file for BatchBuilder class
 *
 * @file BatchBuilder.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_BATCH_BUILDER_H
#define MAGIC3D_BATCH_BUILDER_H

#include "../Exceptions/MagicException.h"
#include "../Shaders/Shader.h"
#include "VertexArray.h"
#include "Batch.h"
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
class BatchBuilder
{
private:	
    /// datatype used when building model by hand, before end() call
	struct BuildData
	{
		float* data;
		int currentIndex;
		int currentVertex;
		
		inline BuildData(Batch::AttributeData* data)
		{
		    this->data = data->data; // data indeed!
		    this->currentIndex = 0;
		    this->currentVertex = 0;
		}
		
		inline void setCurrentVertex(int cur, int comps)
		{
		    this->currentVertex = cur;
		    this->currentIndex = cur * comps;
		}
		    
	};
    
	/** mapping of attribute build index to build data
	 */
	BuildData* buildData[Shader::MAX_ATTRIBUTE_TYPES];
	
	/// the batch currently being built
	Batch* batch;
	
	/// number of verticies being managed
	int vertexCount;
	
	/// number of current attributes
	int curAttributeCount;
	
	inline BuildData* setupBuildData(Shader::AttributeType type)
	{
	    MAGIC_THROW( batch == NULL, "Not currently in a build sequence." );
	    
	    BuildData* b = this->buildData[(int)type];
	    if (b != NULL)
	        return b; // already setup
	    
	    // make sure we haven't exceeded attribute limit
	    MAGIC_THROW(curAttributeCount >= batch->attributeCount, 
	        "Exceeded specified attribute limit on batch." );
	    
	    // get next unallocate attribute data instance
	    Batch::AttributeData* data = NULL;
	    for (int i=0; i < batch->attributeCount; i++)
	    {
	        if (batch->data[i].data == NULL)
	        {
	            data = &batch->data[i];
	            break;
	        }
	    }
	    MAGIC_ASSERT(data != NULL);
	    
	    // allocate new attribute data
	    data->allocate(this->vertexCount, type);
	    curAttributeCount++;
	    
	    // create build data and return it
	    this->buildData[(int)type] = new BuildData(data);
	    return this->buildData[(int)type];
	}
	
	inline BuildData* getBuildData(Shader::AttributeType type)
	{
	    MAGIC_THROW( batch == NULL, "Not currently in a build sequence." );
	    
	    BuildData* b = this->buildData[(int)type];
	    MAGIC_THROW(b == NULL, "Tried to get data for unused attribute." );
	    
	    return b;
	}
	
	inline void setAttribute4(BuildData* data, float c1, float c2, float c3, float c4)
	{
	    MAGIC_THROW(data->currentVertex >= this->vertexCount, "Went over specified "
	        "vertex count when building a mesh.");
		data->data[ data->currentIndex   ] = c1;
		data->data[ data->currentIndex+1 ] = c2;
		data->data[ data->currentIndex+2 ] = c3;
		data->data[ data->currentIndex+3 ] = c4;
		data->currentIndex += 4;
		data->currentVertex++;
	}
	
	inline void setAttributeNv(BuildData* data, int count, int comps, float* cv)
	{
	    MAGIC_THROW((data->currentVertex + count) > this->vertexCount, "Went over specified "
	        "vertex count when building a mesh.");
	    memcpy(&data->data[data->currentIndex], cv, count*comps*sizeof(float));
	    data->currentIndex += count*comps;
	    data->currentVertex += count;
	}
	
	inline void setAttribute3(BuildData* data, float c1, float c2, float c3)
	{		
	    MAGIC_THROW(data->currentVertex >= this->vertexCount, "Went over specified "
	        "vertex count when building a mesh.");
		data->data[ data->currentIndex   ] = c1;
		data->data[ data->currentIndex+1 ] = c2;
		data->data[ data->currentIndex+2 ] = c3;
		data->currentIndex += 3;
		data->currentVertex++;
	}
	
	inline void setAttribute2(BuildData* data, float c1, float c2)
	{
	    MAGIC_THROW(data->currentVertex >= this->vertexCount, "Went over specified "
	        "vertex count when building a mesh.");
		data->data[ data->currentIndex   ] = c1;
		data->data[ data->currentIndex+1 ] = c2;
		data->currentIndex += 2;
		data->currentVertex++;
	}
	
	inline void getAttribute4(BuildData* data, float* c1, float* c2, float* c3, float* c4)
	{
		(*c1) = data->data[ data->currentIndex   ];
		(*c2) = data->data[ data->currentIndex+1 ];
		(*c3) = data->data[ data->currentIndex+2 ];
		(*c4) = data->data[ data->currentIndex+3 ];
	}
	
	inline void getAttribute3(BuildData* data, float* c1, float* c2, float* c3)
	{		
	    (*c1) = data->data[ data->currentIndex   ];
		(*c2) = data->data[ data->currentIndex+1 ];
		(*c3) = data->data[ data->currentIndex+2 ];
	}
	
	inline void getAttribute2(BuildData* data, float* c1, float* c2)
	{
	    (*c1) = data->data[ data->currentIndex   ];
		(*c2) = data->data[ data->currentIndex+1 ];
	}
	

public:
	/** Standard Constructor
	 */
	BatchBuilder();
	
	/// destructor
	~BatchBuilder();
	
	/** Starts a vertex building sequence
	 * @param vertexCount the number of verticies to be handled
	 */
	void begin(int vertexCount, int attributeCount, Batch* batch);
	
	/** Modify a current batch.
	 * @param batch the batch to modify
	 */
	void modify(Batch* batch);
	
	/** Manually set the current vertex for all currently 
	 * know attributes. Note that any skipped over values are not set
	 * to any specified value and that any new attributes set after this
	 * will start at 0 as normal and not this vertex.
	 * @param currentVertex the vertex to set
	 */
	void setCurrentVertex( int currentVertex );
	
	inline void vertex4f(float v1, float v2, float v3, float v4)
	{
	    this->setAttribute4( this->setupBuildData( Shader::VERTEX ), 
	        v1, v2, v3, v4 );
	}
	
	inline void vertex4fv(float* data, int count)
	{
	    this->setAttributeNv(this->setupBuildData( Shader::VERTEX ), count, 4, data);
	}
	
	inline void vertex3f(float v1, float v2, float v3)
	{
	    this->setAttribute4( this->setupBuildData( Shader::VERTEX ), 
	        v1, v2, v3, 1.0f );
	}
	
	inline void vertex3fv(float* data, int count)
	{
	    // since vertex is actually 4 comps, we can't just memcpy
	    BuildData* b = this->setupBuildData( Shader::VERTEX );
	    for (int i=0; i < count; i++)
	    {
	        this->setAttribute4( b, data[i*3], data[i*3+1], data[i*3+2], 1.0f );
	    }
	}
	
	inline void getVertex3f(float* v1, float* v2, float* v3)
	{
	    float dummy;
	    this->getAttribute4( this->getBuildData( Shader::VERTEX ), 
	        v1, v2, v3, &dummy );
	}
	
	inline void normal3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Shader::NORMAL ), 
	        v1, v2, v3);
	}
	
	inline void normal3fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::NORMAL ), 
	        count, 3, data);
	}
	
	inline void color3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Shader::COLOR ), 
	        v1, v2, v3);
	}
	
	inline void color3fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::COLOR ), 
	        count, 3, data);
	}
	
	inline void color2_3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Shader::COLOR2 ), 
	        v1, v2, v3);
	}
	
	inline void color2_3fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::COLOR2 ), 
	        count, 3, data);
	}
	
	inline void texCoord0_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Shader::TEX_COORD_0 ), 
	        v1, v2);
	}
	
	inline void texCoord0_2fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TEX_COORD_0 ), 
	        count, 2, data);
	}
	
	inline void texCoord2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Shader::TEX_COORD_0 ), 
	        v1, v2);
	}
	
	inline void texCoord2fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TEX_COORD_0 ), 
	        count, 2, data);
	}
	
	inline void texCoord1_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Shader::TEX_COORD_1 ), 
	        v1, v2);
	}
	
	inline void texCoord1_2fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TEX_COORD_1 ), 
	        count, 2, data);
	}
	
	inline void texCoord2_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Shader::TEX_COORD_2 ), 
	        v1, v2);
	}
	
	inline void texCoord2_2fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TEX_COORD_2 ), 
	        count, 2, data);
	}
	
	inline void texCoord3_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Shader::TEX_COORD_3 ), 
	        v1, v2);
	}
	
	inline void texCoord3_2fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TEX_COORD_3 ), 
	        count, 2, data);
	}
	
	inline void texCoord4_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Shader::TEX_COORD_4 ), 
	        v1, v2);
	}
	
	inline void texCoord4_2fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TEX_COORD_4 ), 
	        count, 2, data);
	}
	
	inline void texCoord5_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Shader::TEX_COORD_5 ), 
	        v1, v2);
	}
	
	inline void texCoord5_2fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TEX_COORD_5 ), 
	        count, 2, data);
	}
	
	inline void texCoord6_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Shader::TEX_COORD_6 ), 
	        v1, v2);
	}
	
	inline void texCoord6_2fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TEX_COORD_6 ), 
	        count, 2, data);
	}
	
	inline void texCoord7_2f(float v1, float v2)
	{
	    this->setAttribute2( this->setupBuildData( Shader::TEX_COORD_7 ), 
	        v1, v2);
	}
	
	inline void texCoord7_2fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TEX_COORD_7 ), 
	        count, 2, data);
	}
	
	inline void tangent3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Shader::TANGENT ), 
	        v1, v2, v3);
	}
	
	inline void tangent3fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::TANGENT ), 
	        count, 3, data);
	}
	
	inline void binormal3f(float v1, float v2, float v3)
	{
	    this->setAttribute3( this->setupBuildData( Shader::BINORMAL ), 
	        v1, v2, v3);
	}
	
	inline void binormal3fv(float* data, int count)
	{
	    this->setAttributeNv( this->setupBuildData( Shader::BINORMAL ), 
	        count, 3, data);
	}
	
	/** end a vertex building sequence
	 */
	void end();
	
	/** Build a box mesh
     * @param width the width of the box
     * @param height the height of the box
     * @param depth the depth of the box
     */
	void buildBox(Batch* mesh, float width, float height, float depth );
	
	/** Build a 2D circle mesh
     * @param x the x coordinate of the center
     * @param y the y coordinate of the center
     * @param radius the radius of the circle
     * @param precisionAngle the angle between any two points on the edge of the
                            circle, the lower angle, the better looking
     */
	void build2DCircle(Batch* mesh, int x, int y, int radius, float precisionAngle );
	
	/** Build a flat surface
     * @param width the width of the surface
     * @param height the height of the surface
     * @param slices the number of squares on width
     * @param stacks the number of squares on height
     */
    void buildFlatSurface(Batch* mesh, float width, float height, int slices, 
        int stacks, bool texRepeat, float texPerX, float texPerY);

    /** Build 2D rectangle
     * @param x the upper-left x coordinate of the rectangle
     * @param y the upper-left y coordinate of the rectangle
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     */
    void build2DRectangle(Batch* mesh, int x, int y, int width, int height);
    
    /** Build sphere
     * @param radius the radius of the sphere
     * @param slices the number of squares on width
     * @param stacks the number of squares on height
     */
    void buildSphere(Batch* mesh, float radius, int slices, int stacks);
    
};


};




#endif
















