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
 * @file PointLightDiffuseShader.cpp
 * @author Andrew Keating
 */

#include <Shaders/PointLightDiffuseShader.h>
#include <Exceptions/MagicException.h>

namespace Magic3D
{
	
const char* PointLightDiffuseShader::vertexSourceName = "shaders/PointLightDiffuseShader.vp";
const char* PointLightDiffuseShader::fragmentSourceName = "shaders/PointLightDiffuseShader.fp";
	
/** Standard constructor
 * @param resourceManager the resource manager to use to find shader source files
 */
PointLightDiffuseShader::PointLightDiffuseShader(ResourceManager& resourceManager)
{
	// get vertex and fragment source for shader
	Handle <TextResource> vp = resourceManager.get<TextResource>(PointLightDiffuseShader::vertexSourceName);
	Handle <TextResource> fp = resourceManager.get<TextResource>(PointLightDiffuseShader::fragmentSourceName);
	
	// load the shader
	this->programId = Shader::loadShader(vp()->getText(), fp()->getText(), // vertex and fragment source code
					2, 				// two attributes
					0, "vVertex",	// first has shader var name vVertex
					1, "vNormal"   // second has shader var name vNormal
	);
	
	// just let go of the handles as we don't need to text ever again
}
	
/// destructor
PointLightDiffuseShader::~PointLightDiffuseShader()
{
	// delete the shader from opengl memory
	glDeleteProgram(this->programId);
}
		
	
	
/** Enable this shader to be used on the next drawing operation
 */
void PointLightDiffuseShader::use()
{
	// set opengl to use this shader
	glUseProgram(this->programId);
	if (glGetError() != GL_NO_ERROR)
		throw MagicExceptionMacro("Could not use shader program");
	
	// set uniforms for shader program to use
	
	// set diffuse color
	GLint id = glGetUniformLocation(this->programId, "diffuseColor");
	glUniform4fv(id, 1, diffuseColor.getInternal());
	
	// set light position
    GLfloat tmp[4*4];
    tmp[0] = lightPosition.getX();
    tmp[1] = lightPosition.getY();
    tmp[2] = lightPosition.getZ();
	id = glGetUniformLocation(this->programId, "vLightPosition");
	glUniform3fv(id, 1, tmp);
	
	// set mvp matrix
    mvpMatrix.getArray<GLfloat>(tmp);
	id = glGetUniformLocation(this->programId, "mvpMatrix");
	glUniformMatrix4fv(id, 1, GL_FALSE, tmp);
	
	// set mv matrix
    mvMatrix.getArray<GLfloat>(tmp);
	id = glGetUniformLocation(this->programId, "mvMatrix");
	glUniformMatrix4fv(id, 1, GL_FALSE, tmp);
	
	// set normal matrix
    normalMatrix.getArray<GLfloat>(tmp);
	id = glGetUniformLocation(this->programId, "normalMatrix");
	glUniformMatrix3fv(id, 1, GL_FALSE, tmp);
	
	if (glGetError() != GL_NO_ERROR)
		throw MagicExceptionMacro("Could not bind uniforms for shader");
	
	// should be ready to render now
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};
























