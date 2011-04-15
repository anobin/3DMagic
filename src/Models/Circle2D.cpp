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
/** Implementation file for Circle2D class
 * 
 * @file Circle2D.cpp
 * @author Andrew Keating
 */

#include <Models/Circle2D.h>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Magic3D
{

/** Standard constructor 
 * @param x the x coordinate of the center
 * @param y the y coordinate of the center
 * @param radius the radius of the circle
 * @param precisionAngle the angle between any two points on the edge of the
						circle, the lower angle, the better looking
 */
Circle2D::Circle2D(int x, int y, int radius, float precisionAngle):
	Model(ShaderVertexInterfaceSpec::DefaultSpec_Position_Texture)
{
	// no physics for 2D models
	this->Model::collisionShape = NULL;
	
	// since this is a 2D model, we leave all z coords at 0
	
	// points using a TRIANGLE FAN
	int edges = int(360.0f/precisionAngle);
	data.begin(1 + edges);
	
	// center
	data.texCoord2f(0.5f, 0.5f);
	data.position3f((float)x, (float)y, 0.0f);
	
	// draw verticies around the center
	float theta = (float)(precisionAngle * M_PI / 180.0f);
	double r = radius;
	float x1 = (float)x;
	float y1 = (float)y;
	for (int i=0; i < edges; i++)
	{
		// calc coords for this vertex
		float vx;
		float vy;
		if (i == (edges-1))
		{
			// make the last point and the first the same
			vx = (float)(x1 + (r * cos(0.0)));
			vy = (float)(y1 + (r * sin(0.0)));
		}
		else
		{
			vx = (float)(x1 + (r * cos(float(i)*theta)));
			vy = (float)(y1 + (r * sin(float(i)*theta)));
		}
		data.texCoord2f((float)(0.5f + 0.5f * ((vx-x1)/r)), (float)(0.5f + 0.5f * ((vy-y1)/r)));
		data.position3f(vx, vy, 0.0f);
	}

	// end vertex data
	data.end();
}
	
/// destructor
Circle2D::~Circle2D()
{
	/* intentionall left blank */
}
	
/** get the object's type name
 */
const char* Circle2D::getTypeName()
{
	return "Circle2D";
}


};






