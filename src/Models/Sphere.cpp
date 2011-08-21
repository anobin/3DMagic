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
/** Implementation file for buillt-in Sphere mesh
 *
 * @file Sphere.cpp
 * @author Andrew Keating
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <Graphics/MeshBuilder.h>

namespace Magic3D
{
	
/** Build sphere
 * @param radius the radius of the sphere
 * @param slices the number of squares on width
 * @param stacks the number of squares on height
 */
void MeshBuilder::buildSphere(Mesh* mesh, float radius, int slices, int stacks)
{   
	GLfloat drho = (GLfloat)(M_PI) / (GLfloat) stacks;
    GLfloat dtheta = 2.0f * (GLfloat)(M_PI) / (GLfloat) slices;
	GLfloat ds = 1.0f / (GLfloat) slices;
	GLfloat dt = 1.0f / (GLfloat) stacks;
	GLfloat t = 1.0f;
	GLfloat s = 0.0f;
    GLint i, j;     // Looping variables
    
    this->begin(slices * stacks * 6, mesh);
    
	for (i = 0; i < stacks; i++) 
	{
		GLfloat rho = (GLfloat)i * drho;
		GLfloat srho = (GLfloat)(sin(rho));
		GLfloat crho = (GLfloat)(cos(rho));
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));

        s = 0.0f;
		GLfloat vVertex[4][3];
		GLfloat vNormal[4][3];
		GLfloat vTexture[4][2];

		for ( j = 0; j < slices; j++) 
		{
			GLfloat theta = (j == slices) ? 0.0f : j * dtheta;
			GLfloat stheta = (GLfloat)(-sin(theta));
			GLfloat ctheta = (GLfloat)(cos(theta));
			
			GLfloat x = stheta * srho;
			GLfloat y = ctheta * srho;
			GLfloat z = crho;
        
			vTexture[0][0] = s;
			vTexture[0][1] = t;
			vNormal[0][0] = x;
			vNormal[0][1] = y;
			vNormal[0][2] = z;
			vVertex[0][0] = x * radius;
			vVertex[0][1] = y * radius;
			vVertex[0][2] = z * radius;
			
            x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;

 			vTexture[1][0] = s;
			vTexture[1][1] = t - dt;
			vNormal[1][0] = x;
			vNormal[1][1] = y;
			vNormal[1][2] = z;
			vVertex[1][0] = x * radius;
			vVertex[1][1] = y * radius;
			vVertex[1][2] = z * radius;
			

			theta = ((j+1) == slices) ? 0.0f : (j+1) * dtheta;
			stheta = (GLfloat)(-sin(theta));
			ctheta = (GLfloat)(cos(theta));
			
			x = stheta * srho;
			y = ctheta * srho;
			z = crho;
        
            s += ds;
			vTexture[2][0] = s;
			vTexture[2][1] = t;
			vNormal[2][0] = x;
			vNormal[2][1] = y;
			vNormal[2][2] = z;
			vVertex[2][0] = x * radius;
			vVertex[2][1] = y * radius;
			vVertex[2][2] = z * radius;
			
            x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;

 			vTexture[3][0] = s;
			vTexture[3][1] = t - dt;
			vNormal[3][0] = x;
			vNormal[3][1] = y;
			vNormal[3][2] = z;
			vVertex[3][0] = x * radius;
			vVertex[3][1] = y * radius;
			vVertex[3][2] = z * radius;
		
			for (int k=0; k < 3; k++)
			{
				texCoord2f(vTexture[k][0], vTexture[k][1]);
				normal3f(vNormal[k][0], vNormal[k][1], vNormal[k][2]);
				vertex3f(vVertex[k][0], vVertex[k][1], vVertex[k][2]);
			}
			
			// Rearrange for next triangle
			memcpy(vVertex[0], vVertex[1], sizeof(GLfloat)*3);
			memcpy(vNormal[0], vNormal[1], sizeof(GLfloat)*3);
			memcpy(vTexture[0], vTexture[1], sizeof(GLfloat)*2);
			
			memcpy(vVertex[1], vVertex[3], sizeof(GLfloat)*3);
			memcpy(vNormal[1], vNormal[3], sizeof(GLfloat)*3);
			memcpy(vTexture[1], vTexture[3], sizeof(GLfloat)*2);
					
			for (int k=0; k < 3; k++)
			{
				texCoord2f(vTexture[k][0], vTexture[k][1]);
				normal3f(vNormal[k][0], vNormal[k][1], vNormal[k][2]);
				vertex3f(vVertex[k][0], vVertex[k][1], vVertex[k][2]);
			}			
		}
        t -= dt;
	}
	this->end();
}	
	
	
	
	
	
	
	
	
	
	
	
};