/* 
Copyright (c) 2015 Andrew Keating

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
#ifndef MAGIC3D_MODEL_H
#define MAGIC3D_MODEL_H

#include <Graphics\Mesh.h>
#include <CollisionShapes\BoxCollisionShape.h>
#include <Graphics\Material.h>

#include <set>

namespace Magic3D
{


class Model
{
protected:
	std::shared_ptr<Meshes> meshes;
	std::shared_ptr<CollisionShape> collisionShape;
	std::shared_ptr<Material> material;
	
public:
	inline Model() {}

	inline Model(
		std::shared_ptr<Meshes> meshes,
		std::shared_ptr<Material> material,
		std::shared_ptr<CollisionShape> collisionShape = nullptr
		): meshes(meshes), collisionShape(collisionShape), material(material) {}

	inline std::shared_ptr<Meshes> getMeshes()
	{
		return this->meshes;
	}
	inline void setMeshes(std::shared_ptr<Meshes> meshes)
	{
		this->meshes = meshes;
	}
	inline std::shared_ptr<CollisionShape> getCollisionShape()
	{
		return this->collisionShape;
	}
	inline void setCollisionShape(std::shared_ptr<CollisionShape> collisionShape)
	{
		this->collisionShape = collisionShape;
	}
	inline std::shared_ptr<Material> getMaterial()
	{
		return this->material;
	}
	inline void setMaterial(std::shared_ptr<Material> material)
	{
		this->material = material;
	}

};





};



#endif