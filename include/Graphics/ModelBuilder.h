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
/** Header file for ModelBuilder class
 *
 * @file ModelBuilder.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MODEL_BUILDER_H
#define MAGIC3D_MODEL_BUILDER_H

#include "Model.h"

namespace Magic3D
{

/** Builder class for models
 */
class ModelBuilder
{
private:
    /// the model being built
    Model* model;
    
    /// current mesh
    int curMesh;

public:
    inline ModelBuilder(): model(NULL), curMesh(0) {}

    inline void begin(Model* model, int meshCount)
    {
        this->model = model;
        model->allocate(meshCount);
        this->curMesh = 0;
    }
    
    inline void addMesh(Mesh* mesh, Material* material)
    {
        MAGIC_THROW( this->model == NULL, "Tried to add a mesh before calling begin()" );
        MAGIC_THROW( this->curMesh >= this->model->meshCount, "Tried to add more meshes to a model than "
            "the model was specified as having" ); 
        
        model->mesh[curMesh] = mesh;
        model->material[curMesh] = material;
        this->curMesh++;
    }
    
    inline void end()
    {
        MAGIC_THROW(curMesh != model->meshCount, "Called end() without setting all meshes." );
        this->model = NULL;
        this->curMesh = 0;
    }
    
    
    /// convenience method
    inline void buildSimpleModel( Model* model, Mesh* mesh, Material* material )
    {
        this->begin(model, 1);
        this->addMesh(mesh, material);
        this->end();
    }
    
    /// convenience method
    inline void buildModel( Model* model, Mesh** mesh, Material** material, int meshCount )
    {
        this->begin(model, meshCount);
        for(int i=0; i < meshCount; i++)
            this->addMesh(mesh[i], material[i]);
        this->end();
    }

};

};


#endif







