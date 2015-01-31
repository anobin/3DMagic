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
#ifndef MAGIC3D_GPU_PROGRAM_H
#define MAGIC3D_GPU_PROGRAM_H

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

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
#include <Graphics\VertexArray.h>
#include "Shader.h"


namespace Magic3D
{
	
class World;


class GpuProgram
{
public:
    /** different auto-bound attributes types that can be present in mesh data.
     * The shader to render this mesh data does not have to support the exact
     * set of attribute types that are in this mesh, but an attempt will be made
     * to bound the attribute types that are present to the shader with the 
     * specified names and component numbers (type is always float). 
     */
    enum AttributeType
    {
        VERTEX = 0,     // vec4 "vertex"
        NORMAL,         // vec3 "normal"
        COLOR,          // vec4 "color"
        COLOR2,         // vec4 "color2"
        TEX_COORD_0,    // vec2 "texcoord0"
        TEX_COORD_1,    // vec2 "texcoord1"
        TEX_COORD_2,    // vec2 "texcoord2"
        TEX_COORD_3,    // vec2 "texcoord3"
        TEX_COORD_4,    // vec2 "texcoord4"
        TEX_COORD_5,    // vec2 "texcoord5"
        TEX_COORD_6,    // vec2 "texcoord6"
        TEX_COORD_7,    // vec2 "texcoord7"
        TANGENT,        // vec3 "tangent"
        BINORMAL,       // vec3 "binormal"
        MAX_ATTRIBUTE_TYPES
    };

	enum AutoUniformType
    {
        MODEL_MATRIX,                   // mat4
        VIEW_MATRIX,                    // mat4
        PROJECTION_MATRIX,              // mat4
        MODEL_VIEW_MATRIX,              // mat4
        VIEW_PROJECTION_MATRIX,         // mat4
        MODEL_PROJECTION_MATRIX,        // mat4
        MODEL_VIEW_PROJECTION_MATRIX,   // mat4
        NORMAL_MATRIX,                  // mat3
        FPS,                            // int
        TEXTURE0,                       // sampler2D
        TEXTURE1,                       // sampler2D
        TEXTURE2,                       // sampler2D
        TEXTURE3,                       // sampler2D
        TEXTURE4,                       // sampler2D
        TEXTURE5,                       // sampler2D
        TEXTURE6,                       // sampler2D
        TEXTURE7,                       // sampler2D
        LIGHT_LOCATION,                 // vec3
		FLAT_PROJECTION,				// mat4
        MAX_AUTO_UNIFORM_TYPE
    };

	/// number of components for shader variables for each of the auto-bound attribute types
    static const int attributeTypeCompCount[ MAX_ATTRIBUTE_TYPES ];

protected:
	friend class World;

	struct AutoUniform
    {
		std::string varName;
        AutoUniformType type;
        
        inline AutoUniform() {}

        inline AutoUniform(const AutoUniform& u): varName(u.varName), 
			type(u.type) {}

        inline void set(const AutoUniform& u)
        {
            this->type = u.type;
			this->varName = u.varName;
        }
    };
    
	// TODO: refactor this and place in seperate class
    struct NamedUniform
    {
        std::string varName;
        VertexArray::DataTypes datatype;
        int comp_count;
        void* data;
        
        inline NamedUniform(): comp_count(0), data(NULL) {}

        inline ~NamedUniform()
        {
			delete[] data;
        }
    };

    /// id of compiled and linked shader program
    GLuint programId;
    
    /// next index to use for arribute bind
    int nextIndex;

	std::vector<std::shared_ptr<AutoUniform>> autoUniforms;

	std::vector<std::shared_ptr<NamedUniform>> namedUniforms;

	std::shared_ptr<Shader> vertexShader;
	std::shared_ptr<Shader> fragmentShader;
    
	/// default constructor
	inline GpuProgram() { /* intentionally left blank */ }

public:
	GpuProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader);
    
	/// destructor
	virtual ~GpuProgram();

	/** Enable this shader to be used on the next drawing operation
     * and for setting uniforms
	 */
	void use();
	
	inline void bindAttrib(const char* name, AttributeType type)
	{
	    glBindAttribLocation(programId, (int)type, name);
	    
	    MAGIC_THROW( glGetError() != GL_NO_ERROR, "Failed to bind attribute." );
	    
	    nextIndex++;
	}

	void addAutoUniform(const char* varName, AutoUniformType type)
	{
		auto u = std::make_shared<AutoUniform>();
		u->varName = varName;
		u->type = type;
		this->autoUniforms.push_back(u);
	}

	std::vector<std::shared_ptr<AutoUniform>> getAutoUniforms()
	{
		return this->autoUniforms;
	}
    
    void addNamedUniform(const char* varName, VertexArray::DataTypes datatype,
        int comp_count, const void* data)
	{
		auto u = std::make_shared<NamedUniform>();
		u->varName = varName;
		u->datatype = datatype;
		u->comp_count = comp_count;
		u->data = new char[VertexArray::getDataTypeSize(datatype)*comp_count];
		memcpy(u->data, data, VertexArray::getDataTypeSize(datatype)*comp_count);
		this->namedUniforms.push_back(u);
	}

	std::vector<std::shared_ptr<NamedUniform>> getNamedUniforms()
	{
		return this->namedUniforms;
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











