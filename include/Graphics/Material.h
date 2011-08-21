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
/** Header file for Material class
 *
 * @file Material.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MATERIAL_H
#define MAGIC3D_MATERIAL_H

#include "VertexArray.h"
#include "../Shaders/Shader.h"
#include "Texture.h"

namespace Magic3D
{

class MaterialBuilder;
class World;

/** Contains all data on how to render a mesh including the shader to use, 
 * attributes to pass to the shader, uniforms to pass to the shader, 
 * the primitive to render, etc.
 */
class Material
{
public:
    enum AutoUniformType
    {
        MODEL_MATRIX,
        VIEW_MATRIX,
        PROJECTION_MATRIX,
        MODEL_VIEW_MATRIX,
        VIEW_PROJECTION_MATRIX,
        MODEL_PROJECTION_MATRIX,
        MODEL_VIEW_PROJECTION_MATRIX,
        NORMAL_MATRIX,
        FPS,
        TEXTURE0,
        TEXTURE1,
        TEXTURE2,
        TEXTURE3,
        TEXTURE4,
        TEXTURE5,
        TEXTURE6,
        TEXTURE7,
        MAX_AUTO_UNIFORM_TYPE
    };
    
    
protected:
    friend class MaterialBuilder;
    friend class World;
    
    struct AutoUniform
    {
        char* varName;
        AutoUniformType type;
        
        inline AutoUniform(): varName(NULL) {}
        
        inline ~AutoUniform()
        {
            delete[] varName;
        }
    };
    
    struct NamedUniform
    {
        char* varName;
        VertexArray::DataTypes datatype;
        int comp_count;
        const void* data;
        
        inline NamedUniform(): varName(NULL), comp_count(0), data(NULL) {}
        
        inline ~NamedUniform()
        {
            delete[] varName;
        }
    };
    
    Shader* shader;
    
    VertexArray::Primitives primitive;
    
    bool transparent;
    
    AutoUniform* uniforms;
    
    int autoUniformCount;
    
    NamedUniform* namedUniforms;
    
    int namedUniformCount;
    
    Texture* textures[8];
    
    inline void allocate(int autoUniformCount, int namedUniformCount)
    {
        delete[] this->uniforms;
        delete[] this->namedUniforms;
        
        this->autoUniformCount = autoUniformCount;
        this->namedUniformCount = namedUniformCount;
        this->shader = NULL;
        this->primitive = VertexArray::TRIANGLES;
        this->transparent = false;
        this->uniforms = new AutoUniform[autoUniformCount];
        this->namedUniforms = new NamedUniform[autoUniformCount];
        for(int i=0; i < 8; i++)
            this->textures[i] = NULL;
    }
    
    
public:
    inline Material(): shader(NULL), primitive(VertexArray::TRIANGLES), transparent(false),
        uniforms(NULL), autoUniformCount(0), namedUniforms(NULL), namedUniformCount(0)
    {
        for(int i=0; i < 8; i++)
            this->textures[i] = NULL;
    }
    
    inline ~Material()
    {
        delete[] this->uniforms;
        delete[] this->namedUniforms;
    }
    
    
};


};























#endif



