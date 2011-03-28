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
/** Header file for Shader2D class
 *
 * @file Shader2D.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_SHADER_2D_H
#define MAGIC3D_SHADER_2D_H

#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif

#include "Shader.h"
#include "../Math/Matrix4.h"
#include "../Util/Color.h"
#include "../Resources/TextResource.h"
#include "../Resources/ResourceManager.h"
#include "../Graphics/Texture.h"

namespace Magic3D
{

/** Represents a shader that simulates outside hemisphere shading
 * and applies a 2D texture
 */
class Shader2D : public Shader
{
protected:
	/// model view projection matrix
	Matrix4 mvpMatrix;
	/// the texture to shade with
	Texture* texture;
	
	/// id of compiled and linked shader program
	GLuint programId;
	
	/// name of vertex shader program
	static const char* vertexSourceName;
	/// name of fragment shader program
	static const char* fragmentSourceName;
	

	/// default constructor
	inline Shader2D(): texture(NULL) {}
	
	
public:
	/** Standard constructor
	 * @param resourceManager the resource manager to use to find shader source files
	 */
	Shader2D(ResourceManager& resourceManager);
	
	/// destructor
	virtual ~Shader2D();
	
	
	inline void setMVPMatrix(const Matrix4& mvp)
	{
		mvpMatrix.set(mvp);
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

















