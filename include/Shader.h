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
/** Header file for Shader class
 *
 * @file Shader.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_SHADER_H
#define MAGIC3D_SHADER_H

// include opengl
#include <glew.h>
#include <gl.h>


namespace Magic3D
{
	
/** Base class for all shaders
 */
class Shader
{
protected:
	/// default constructor
	inline Shader() { /* intentionally left blank */ }

public:
	/// destructor
	virtual ~Shader();


	/** Enable this shader to be used on the next drawing operation
	 */
	virtual void use() = 0;


	/** load, compile, link, and attach shader
	 * @param vertexProgram source of the vertex program
	 * @param fragmentProgram source of the fragment program
	 * @param ... number of attributes followed by attribute pairs
	 * @return id of compiled shader
	 */
	static GLuint loadShader(const char* vertexProgram, const char* fragmentProgram, ...);

};

	
};



#endif











