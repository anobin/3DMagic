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
/** Header file for HemisphereTexShader class
 *
 * @file HemisphereTexShader.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_HEMISPHERE_TEX_SHADER_H
#define MAGIC3D_HEMISPHERE_TEX_SHADER_H

#include <glew.h>

#include "../Shader.h"
#include "../Point.h"
#include "../Vector.h"
#include "../Matrix.h"
#include "../Color.h"
#include "../Resources/TextResource.h"
#include "../ResourceManager.h"
#include "../Texture.h"

namespace Magic3D
{

/** Represents a shader that simulates outside hemisphere shading
 * and applies a 2D texture
 */
class HemisphereTexShader : public Shader
{
protected:
	/// the color emitted by the sky/light source
	Color skyColor;
	/// the color emitted by the ground/away from light source
	Color groundColor;
	/// the position of the single light
	Point3f lightPosition;
	/// the normal matrix to transform the normal to eye coords
	Matrix33f normalMatrix;
	/// model view projection matrix
	Matrix44f mvpMatrix;
	/// model view matrix
	Matrix44f mvMatrix;
	/// the texture to shade with
	Texture* texture;
	
	/// id of compiled and linked shader program
	GLuint programId;
	
	/// name of vertex shader program
	static const char* vertexSourceName;
	/// name of fragment shader program
	static const char* fragmentSourceName;
	

	/// default constructor
	inline HemisphereTexShader(): texture(NULL) {}
	
	
public:
	/** Standard constructor
	 * @param resourceManager the resource manager to use to find shader source files
	 */
	HemisphereTexShader(ResourceManager& resourceManager);
	
	/// destructor
	virtual ~HemisphereTexShader();
	
	
	
	inline void setSkyColor(const Color& color)
	{
		skyColor.set(color);
	}
	inline void setGroundColor(const Color& color)
	{
		groundColor.set(color);
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
	inline void setTexture(Texture& tex)
	{
		texture = &tex;
	}


	/** Enable this shader to be used on the next drawing operation
	 */
	virtual void use();


};



};





#endif

















