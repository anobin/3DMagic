/* 
Copyright (c) 2015 Andrew Keating

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

#include <Shaders/Shader.h>
#include <Exceptions\ShaderCompileException.h>


namespace Magic3D
{


Shader::Shader( const char* shaderText, Shader::Type type)
{
    id = glCreateShader((GLenum)type);
	
    // Load shader text
	GLchar* shaderListing[1];
	shaderListing[0] = (GLchar*)shaderText;
	glShaderSource(id, 1, (const GLchar**)shaderListing, nullptr);
   
    // Compile shader
    glCompileShader(id);
    
    // Check for compile errors
	GLint ret;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ret);
    if(ret == GL_FALSE)
        throw_ShaderCompileException("Shader failed to compile");
}

Shader::~Shader()
{
    glDeleteShader(id);
}


};