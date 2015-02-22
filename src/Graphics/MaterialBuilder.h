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
/** Header file for MaterialBuilder class
 *
 * @file MaterialBuilder.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MATERIAL_BUILDER_H
#define MAGIC3D_MATERIAL_BUILDER_H


#include "Material.h"

namespace Magic3D
{


/** Builds materials
 */
class MaterialBuilder
{
private:
    /// the material being built
    Material* material;
public:
    inline MaterialBuilder(): material(NULL) {}
    
    inline void begin(Material* material)
    {
        this->material = material;
    }

    inline void modify(Material* material)
    {
        this->material = material;
    }

    inline void expand(Material* material, const Material& basis)
    {
        this->material = material;
        this->material->set(basis);
    }
    
    inline void setGpuProgram(std::shared_ptr<GpuProgram> gpuProgram)
    {
        MAGIC_THROW(material == NULL, "Tried to modify material without calling begin().");
        material->gpuProgram = gpuProgram;
    }
    
    inline void setTransparentFlag(bool transparent)
    {
        MAGIC_THROW(material == NULL, "Tried to modify material without calling begin().");
        material->transparent = transparent;
    }
    
    inline void setTexture(std::shared_ptr<Texture> t, int number = 0)
    {
        MAGIC_THROW(material == NULL, "Tried to modify material without calling begin().");
        MAGIC_THROW(number < 0 || number > 7, "Tried to set out-of-range texture." );
        material->textures[number] = t;
    }
    
    /// the ammount to lie to the depth buffer when rendering object
    inline void setDepthBufferLie(float lie)
    {
        MAGIC_THROW(material == NULL, "Tried to modify material without calling begin().");
        material->depthBufferLie = lie;
    }

    inline void setNormalMap(std::shared_ptr<Texture> tex)
    {
        material->normalMap = tex;
    }

    inline void setShininess(Scalar shininess)
    {
        material->shininess = shininess;
    }
    
    void end();
    
    
};



};

























#endif


