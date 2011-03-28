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
/** Implementation file for Shader2D class
 *
 * @file Shader2D.cpp
 * @author Andrew Keating
 */

#include <Shaders/Shader2D.h>

namespace Magic3D
{
	
const char* Shader2D::vertexSourceName = "shaders/Shader2D.vp";
const char* Shader2D::fragmentSourceName = "shaders/Shader2D.fp";
	
/** Standard constructor
 * @param resourceManager the resource manager to use to find shader source files
 */
Shader2D::Shader2D(ResourceManager& resourceManager)
{
	// get vertex and fragment source for shader
	Handle<TextResource> vp = resourceManager.get<TextResource>(Shader2D::vertexSourceName);
	Handle<TextResource> fp = resourceManager.get<TextResource>(Shader2D::fragmentSourceName);
	
	// load the shader
	this->programId = Shader::loadShader(vp()->getText(), fp()->getText(), // vertex and fragment source code
					2, 				// two attributes
					0, "location",	// first has shader var name location
					3, "texCoord" // thrid has shader var name texCoord
	);
	
	// just let go of the handles as we don't need to text ever again
}
	
/// destructor
Shader2D::~Shader2D()
{
	// delete the shader from opengl memory
	glDeleteProgram(this->programId);
}
		
	
	
/** Enable this shader to be used on the next drawing operation
 */
void Shader2D::use()
{
	// set opengl to use this shader
	glUseProgram(this->programId);
	if (glGetError() != GL_NO_ERROR)
		throw MagicExceptionMacro("Could not use shader program");
	
	// set uniforms for shader program to use
	
	// set mv matrix
	GLuint id;
    GLfloat tmp[4*4];
    for (int i=0; i < 4*4; i++)
        tmp[i] = mvpMatrix.get(i/4, i%4);
	id = glGetUniformLocation(this->programId, "mvpMatrix");
	glUniformMatrix4fv(id, 1, GL_FALSE, tmp);

	// set texture map
	texture->bind();
	id = glGetUniformLocation(this->programId, "textureMap");
	glUniform1i(id, 0); // use texture state 0
	
	if (glGetError() != GL_NO_ERROR)
		throw MagicExceptionMacro("Could not bind uniforms for shader");
	
	// should be ready to render now
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};


