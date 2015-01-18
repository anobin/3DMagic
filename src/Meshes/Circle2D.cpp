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
/** Implementation file for built-in Circle2D batch
 * 
 * @file Circle2D.cpp
 * @author Andrew Keating
 */

#define _USE_MATH_DEFINES
#include <math.h>

#include <Graphics/BatchBuilder.h>

namespace Magic3D
{

/** Build a 2D circle batch
 * @param x the x coordinate of the center
 * @param y the y coordinate of the center
 * @param radius the radius of the circle
 * @param precisionAngle the angle between any two points on the edge of the
                        circle, the lower angle, the better looking
 */
void BatchBuilder::build2DCircle(Batch* batch, int x, int y, int radius, float precisionAngle )
{
	
	// since this is a 2D model, we leave all z coords at 0
	
	// points using a TRIANGLE FAN
	int edges = int(360.0f/precisionAngle);
	this->begin(1 + edges, 2, batch);
	
	// center
	texCoord2f(0.5f, 0.5f);
	vertex3f((float)x, (float)y, 0.0f);
	
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
		texCoord2f((float)(0.5f + 0.5f * ((vx-x1)/r)), (float)(0.5f + 0.5f * ((vy-y1)/r)));
		vertex3f(vx, vy, 0.0f);
	}

	// end vertex data
	this->end();
}

};






