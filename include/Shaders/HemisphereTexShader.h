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

#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif

#include "Shader.h"
#include "../Math/Point3.h"
#include "../Math/Matrix4.h"
#include "../Math/Matrix3.h"
#include "../Util/Color.h"
#include "../Resources/TextResource.h"
#include "../Resources/ResourceManager.h"
#include "../Graphics/Texture.h"

namespace Magic3D
{

/** Represents a shader that simulates outside hemisphere shading
 * and applies a 2D texture
 */
class HemisphereTexShader : public Shader
{
protected:
	/// the color emitted by the sky/light source
	const GLfloat* skyColor;
	/// the color emitted by the ground/away from light source
	const GLfloat* groundColor;
	/// the position of the single light
	GLfloat lightPosition[3];
	/// the normal matrix to transform the normal to eye coords
	const GLfloat* normalMatrix;
	/// model view projection matrix
	const GLfloat* mvpMatrix;
	/// model view matrix
	const GLfloat* mvMatrix;
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
		skyColor = color.getInternal();
	}
	inline void setGroundColor(const Color& color)
	{
		groundColor = color.getInternal();
	}
	inline void setLightPosition(const Point3& position)
	{
		lightPosition[0] = position.getX();
		lightPosition[1] = position.getY();
		lightPosition[2] = position.getZ();
	}
	inline void setNormalMatrix(const Matrix3& normal)
	{
		normalMatrix = normal.getArray();
	}
	inline void setMVPMatrix(const Matrix4& mvp)
	{
		mvpMatrix = mvp.getArray();
	}
	inline void setMVMatrix(const Matrix4& mv)
	{
		mvMatrix = mv.getArray();
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

















