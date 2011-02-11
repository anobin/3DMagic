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
/** Header file for PointLightDiffuseShader class
 *
 * @file PointLightDiffuseShader.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POINT_LIGHT_DIFFUSE_SHADER_H
#define MAGIC3D_POINT_LIGHT_DIFFUSE_SHADER_H

#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif

#include "../Shader.h"
#include "../Point.h"
#include "../Vector.h"
#include "../Matrix.h"
#include "../Color.h"
#include "../Resources/TextResource.h"
#include "../ResourceManager.h"

namespace Magic3D
{

/** Represents a shader that simulates a single
 * point light that spreads diffuse light in all
 * directions. Uses vertex and normal attributes.
 */
class PointLightDiffuseShader : public Shader
{
protected:
	/// the color to diffuse with
	Color diffuseColor;
	/// the position of the single light
	Point3f lightPosition;
	/// the normal matrix to transform the normal to eye coords
	Matrix33f normalMatrix;
	/// model view projection matrix
	Matrix44f mvpMatrix;
	/// model view matrix
	Matrix44f mvMatrix;
	
	/// id of compiled and linked shader program
	GLuint programId;
	
	/// name of vertex shader program
	static const char* vertexSourceName;
	/// name of fragment shader program
	static const char* fragmentSourceName;
	

	/// default constructor
	inline PointLightDiffuseShader() {}
	
	
public:
	/** Standard constructor
	 * @param resourceManager the resource manager to use to find shader source files
	 */
	PointLightDiffuseShader(ResourceManager& resourceManager);
	
	/// destructor
	virtual ~PointLightDiffuseShader();
	
	
	
	inline void setDiffuseColor(const Color& color)
	{
		diffuseColor.set(color);
	}
	inline void setLightPosition(const Point3f& position)
	{
		lightPosition.set(position);
	}
	inline void setNormalMatrix(const Matrix33f& normal)
	{
		normalMatrix.set(normal);
	}
	inline void setMVPMatrix(const Matrix44f& mvp)
	{
		mvpMatrix.set(mvp);
	}
	inline void setMVMatrix(const Matrix44f& mv)
	{
		mvMatrix.set(mv);
	}


	/** Enable this shader to be used on the next drawing operation
	 */
	virtual void use();


};



};





#endif

















