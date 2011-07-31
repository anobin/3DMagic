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
#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif

// local includes
#include "../Math/MathTypes.h"
#include "../Exceptions/MagicException.h"
#include "../Graphics/Texture.h"
#include "VertexAttribSpec.h"
#include "../Exceptions/ShaderCompileException.h"


namespace Magic3D
{
	
/** Base class for all shaders
 */
class Shader
{
protected:
    /// id of compiled and linked shader program
    GLuint programId;
    
    /// Vertex attribute specification
    VertexAttribSpec spec;
    
    /// index for next named attribute to be added
    int nextNamedIndex;
    
	/// default constructor
	inline Shader() { /* intentionally left blank */ }
	

public:
    /** Create shader
     * @param vertexProgram source of the vertex program
     * @param fragmentProgram source of the fragment program
     * @param ... number of attributes followed by attribute pairs
     */
    Shader( const char* vertexProgram, const char* fragmentProgram);
    
	/// destructor
	virtual ~Shader();



	/** Enable this shader to be used on the next drawing operation
     * and for setting uniforms
	 */
	void use();
	
	inline void bindBuiltInAttrib( VertexAttribSpec::BuiltInAttributeType attrib, const char* name )
	{
	    glBindAttribLocation(programId, (int) attrib , name);
	    
	    // TODO: add verification for variable matches built-in type and component number
	    
	    spec.setBuiltIn( attrib, (int) attrib );
	}
	
	inline void bindNamedAttrib( const char* attribName, const char* name, int components, VertexArray::DataTypes type )
	{
	    glBindAttribLocation(programId, nextNamedIndex , name);
	    
	    VertexAttribSpec::NamedAttribType data;
	    data.index = nextNamedIndex;
	    data.components = components;
	    data.type = type;
	    
	    spec.setNamed( name, data );
	    
	    nextNamedIndex++;
	}
	
	inline void link()
	{
	    GLint ret;
	    
	    // link the compiled shader program
        glLinkProgram(programId);
        
        // check for link errors
        glGetProgramiv(programId, GL_LINK_STATUS, &ret);
        if(ret == GL_FALSE)
        {
            glDeleteProgram(programId);
            throw_ShaderCompileException("Shader Program failed to link");
        }
	}
	
	inline const VertexAttribSpec* getVertexAttribSpec()
	{
	    return &spec;
	}

    
    inline void setUniformfv( const char* name, int components, const Scalar* values, int count = 1 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        switch( components )
        {
            case 1: glUniform1fv(id, count, values); break;
            case 2: glUniform2fv(id, count, values); break;
            case 3: glUniform3fv(id, count, values); break;
            case 4: glUniform4fv(id, count, values); break;
            default:
                throw_MagicException("Attempt to set uniform with invalid component size");
        }
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind float uniform for shader");    
    }
    
    inline void setUniformf( const char* name, const Scalar v1 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        glUniform1f( id, v1);
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind float uniform for shader");    
    }
    
    inline void setUniformf( const char* name, const Scalar v1, const Scalar v2 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        glUniform2f( id, v1, v2);
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind float uniform for shader");    
    }
    
    inline void setUniformf( const char* name, const Scalar v1, const Scalar v2, const Scalar v3 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        glUniform3f( id, v1, v2, v3);
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind float uniform for shader");    
    }
    
    inline void setUniformf( const char* name, const Scalar v1, const Scalar v2, const Scalar v3, Scalar v4 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        glUniform4f( id, v1, v2, v3, v4);
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind float uniform for shader");    
    }
    
    inline void setUniformiv( const char* name, int components, const int* values, int count = 1 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        switch( components )
        {
            case 1: glUniform1iv(id, count, values); break;
            case 2: glUniform2iv(id, count, values); break;
            case 3: glUniform3iv(id, count, values); break;
            case 4: glUniform4iv(id, count, values); break;
            default:
                throw_MagicException("Attempt to set uniform with invalid component size");
        }
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind integer uniform for shader");    
    }
    
    inline void setUniformMatrix( const char* name, int components, const Scalar* values, int count = 1 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        switch( components )
        {
            case 2: glUniformMatrix2fv(id, count, GL_FALSE, values); break;
            case 3: glUniformMatrix3fv(id, count, GL_FALSE, values); break;
            case 4: glUniformMatrix4fv(id, count, GL_FALSE, values); break;
            default:
                throw_MagicException("Attempt to set matrix uniform with invalid component size");
        }
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind matrix uniform for shader");    
    }
    
    inline void setTexture( const char* name, Texture* tex)
    {
        tex->bind();
        GLint id = glGetUniformLocation(this->programId, name);
        glUniform1i( id, 0 );
        
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind texture uniform for shader");
    }
    
	

};

	
};



#endif











