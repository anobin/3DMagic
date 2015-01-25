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
#include <Shaders\GpuProgram.h>
#include "Texture.h"
#include <string.h>
#include <memory>

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
protected:
    friend class MaterialBuilder;
    friend class World;
    
    std::shared_ptr<GpuProgram> gpuProgram;
    
    VertexArray::Primitives primitive;
    
    bool transparent;
    
    std::shared_ptr<Texture> textures[8];
    
    float depthBufferLie;
    
    inline void allocate()
    {
        this->gpuProgram = NULL;
        this->primitive = VertexArray::TRIANGLES;
        this->transparent = false;
        for(int i=0; i < 8; i++)
            this->textures[i] = NULL;
        this->depthBufferLie = 0;
    }
    
public:
    inline Material(): gpuProgram(NULL), primitive(VertexArray::TRIANGLES), transparent(false),
		depthBufferLie(0)
    {
        for(int i=0; i < 8; i++)
            this->textures[i] = NULL;
    }

	inline Material(const Material& m): gpuProgram(m.gpuProgram), primitive(m.primitive),
        transparent(m.transparent)
    {
        for(int i=0; i < 8; i++)
            this->textures[i] = m.textures[i];
    }

    inline void set(const Material& m)
    {
		this->gpuProgram = m.gpuProgram;
        this->primitive = m.primitive;
        this->transparent = m.transparent;

        for(int i=0; i < 8; i++)
            this->textures[i] = m.textures[i];
        this->depthBufferLie = m.depthBufferLie;
    }    
    
};


};























#endif



