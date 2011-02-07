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
/** Header file for FlatShader class
 *
 * @file FlatShader.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_FLAT_SHADER_H
#define MAGIC3D_FLAT_SHADER_H

#include <glew.h>

#include "../Shader.h"
#include "../Matrix.h"
#include "../Color.h"
#include "../Resources/TextResource.h"
#include "../ResourceManager.h"

namespace Magic3D
{

/** Represents a shader that simply transforms and
 * colors geometry, not good for lighted scenes, but
 * useful for debugging
 */
class FlatShader : public Shader
{
protected:
	/// model view projection matrix
	Matrix44f mvpMatrix;
	/// uniform color value
	Color color;
	
	/// id of compiled and linked shader program
	GLuint programId;
	
	/// name of vertex shader program
	static const char* vertexSourceName;
	/// name of fragment shader program
	static const char* fragmentSourceName;
	

	/// default constructor
	inline FlatShader() {}
	
	
public:
	/** Standard constructor
	 * @param resourceManager the resource manager to use to find shader source files
	 */
	FlatShader(ResourceManager& resourceManager);
	
	/// destructor
	virtual ~FlatShader();
	
	
	
	inline void setColor(const Color& color1)
	{
		this->color.set(color1);
	}
	inline void setMVPMatrix(const Matrix44f& mvp)
	{
		mvpMatrix.set(mvp);
	}

	/** Enable this shader to be used on the next drawing operation
	 */
	virtual void use();


};



};





#endif

















 
