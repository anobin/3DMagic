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
/** Implementation file for MaterialBuilder class
 *
 * @file MaterialBuilder.cpp
 * @author Andrew Keating
 */

#include <Graphics/MaterialBuilder.h>
#include <string.h>

namespace Magic3D
{

void MaterialBuilder::addAutoUniform(const char* varName, Material::AutoUniformType type)
{
    MAGIC_THROW(curAutoUniform >= material->autoUniformCount, "Attempted to add more "
        "auto uniforms than specified." );
    
    Material::AutoUniform* u = &material->uniforms[curAutoUniform];
    u->varName = new char[strlen(varName) + 1];
    strcpy(u->varName, varName);
    u->type = type;
    
    this->curAutoUniform++;
}

void MaterialBuilder::addNamedUniform(const char* varName, VertexArray::DataTypes datatype,
    int comp_count, const void* data)
{
    MAGIC_THROW(curNamedUniform >= material->namedUniformCount, "Attempted to add more "
        "named uniforms than specified." );
    
    Material::NamedUniform* u = &material->namedUniforms[curNamedUniform];
    u->varName = new char[strlen(varName) + 1];
    strcpy(u->varName, varName);
    u->datatype = datatype;
    u->comp_count = comp_count;
    u->data = data;
    
    this->curNamedUniform++;
}

void MaterialBuilder::end()
{
    MAGIC_THROW(curAutoUniform != material->autoUniformCount, "Called end() without setting all "
        "auto uniforms as specified." );
    MAGIC_THROW(curNamedUniform != material->namedUniformCount, "Called end() without setting all "
        "named uniforms as specified." );
    MAGIC_THROW(material->shader == NULL, "Called end() without specifing a shader." );
    
    this->material = NULL;
    this->curAutoUniform = 0;
    this->curNamedUniform = 0;
}





};

























