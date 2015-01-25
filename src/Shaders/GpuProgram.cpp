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

#include <Shaders/GpuProgram.h>


namespace Magic3D
{

const int GpuProgram::attributeTypeCompCount[ MAX_ATTRIBUTE_TYPES ] = 
{
    4, // vertex
    3, // normal
    4, // color
    4, // color2
    2, // texcoord0
    2, // texcoord1
    2, // texcoord2
    2, // texcoord3
    2, // texcoord4
    2, // texcoord5
    2, // texcoord6
    2, // texcoord7
    3, // tangent
    3  // binormal
};


GpuProgram::GpuProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader)
{   
    // create new program and attach compiled shaders
	programId = glCreateProgram();
    glAttachShader(programId, vertexShader->id);
    glAttachShader(programId, fragmentShader->id);
    
    nextIndex = 0;
}

/// destructor
GpuProgram::~GpuProgram()
{
    // delete the shader from opengl memory
    glDeleteProgram(this->programId);
}

/** Enable this shader to be used on the next drawing operation
 * and for setting uniforms
 */
void GpuProgram::use()
{
    // set opengl to use this shader
    glUseProgram(this->programId);
    if (glGetError() != GL_NO_ERROR)
        throw_MagicException("Could not use shader program");
}




};











