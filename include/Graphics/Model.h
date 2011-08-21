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
/** Header file for Model class
 *
 * @file Model.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MODEL_H
#define MAGIC3D_MODEL_H

#include "../Graphics/Mesh.h"
#include "Material.h"
#include "../Shaders/Shader.h"

#include <vector>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace Magic3D
{

class ModelBuilder; 
    

/** Simply a collection of meshes representing a model
 */
class Model
{
protected:
    friend class ModelBuilder;
    
	/// vertex batch(es)
	Mesh** mesh;
	
	/// materials
	Material** material;
	
	/// number of meshes
	int meshCount;
	
	/// (re)allocate model 
	inline void allocate(int meshCount)
	{
	    delete[] mesh;
	    delete[] material;
	    
	    this->meshCount = meshCount;
	    mesh = new Mesh*[meshCount];
	    material = new Material*[meshCount];
	}
	
public:
    /// default constructor
    inline Model(): mesh(NULL), material(NULL), meshCount(0) {}
    
    /// destructor
    ~Model();
    
	inline const Mesh** getMeshData() const
	{
	     return (const Mesh**)mesh;   
	}
	
	inline const Material** getMaterialData() const 
	{
	     return (const Material**)material;   
	}
	
	inline int getMeshCount() const
	{
	    return meshCount;
	}

};





};



#endif





















