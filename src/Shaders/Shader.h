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

#include <map>
#include <string>

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
#include "../Exceptions/ShaderCompileException.h"
#include "../Util/magic_throw.h"


namespace Magic3D
{
	
/** Base class for all shaders
 */
class Shader
{
protected:
    /// id of compiled and linked shader program
    GLuint programId;
    
    /// next index to use for arribute bind
    int nextIndex;
    
    /// attribute indexes
    std::map<std::string, int> attribIndexes;
    
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
	
	inline void bindAttrib( const char* name )
	{
	    glBindAttribLocation(programId, nextIndex , name);
	    
	    MAGIC_THROW( glGetError() != GL_NO_ERROR, "Failed to bind attribute." );
	    
	    attribIndexes.insert( std::pair<std::string, int>( std::string(name), nextIndex ) );
	    
	    nextIndex++;
	}
	
	inline int getAttribBinding( const char* name )
	{
	    std::map<std::string, int>::iterator it = attribIndexes.find(std::string(name));
	    if (it == attribIndexes.end())
	        return -1;
	    return it->second;
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

    
    inline void setUniformfv( const char* name, int components, const Scalar* values, int count = 1 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        MAGIC_THROW( id < 0, "Tried to set a uniform that is not present in shader." );
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
        MAGIC_THROW( id < 0, "Tried to set a uniform that is not present in shader." );
        glUniform1f( id, v1);
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind float uniform for shader");    
    }
    
    inline void setUniformf( const char* name, const Scalar v1, const Scalar v2 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        MAGIC_THROW( id < 0, "Tried to set a uniform that is not present in shader." );
        glUniform2f( id, v1, v2);
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind float uniform for shader");    
    }
    
    inline void setUniformf( const char* name, const Scalar v1, const Scalar v2, const Scalar v3 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        MAGIC_THROW( id < 0, "Tried to set a uniform that is not present in shader." );
        glUniform3f( id, v1, v2, v3);
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind float uniform for shader");    
    }
    
    inline void setUniformf( const char* name, const Scalar v1, const Scalar v2, const Scalar v3, Scalar v4 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        MAGIC_THROW( id < 0, "Tried to set a uniform that is not present in shader." );
        glUniform4f( id, v1, v2, v3, v4);
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind float uniform for shader");    
    }
    
    inline void setUniformiv( const char* name, int components, const int* values, int count = 1 )
    {
        GLint id = glGetUniformLocation(this->programId, name);
        MAGIC_THROW( id < 0, "Tried to set a uniform that is not present in shader." );
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
        MAGIC_THROW( id < 0, "Tried to set a uniform that is not present in shader." );
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
        MAGIC_THROW( id < 0, "Tried to set a uniform that is not present in shader." );
        glUniform1i( id, 0 );
    
        if (glGetError() != GL_NO_ERROR)
            throw_MagicException("Could not bind texture uniform for shader");
    }
    
	

};

	
};



#endif











