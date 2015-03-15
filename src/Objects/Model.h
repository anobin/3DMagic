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

#include <Mesh\TriangleMesh.h>
#include <Graphics\Material.h>

#include <Geometry\Geometry.h>
#include <Geometry\Sphere.h>
#include <Geometry\Box.h>
#include <Geometry\CompoundGeometry.h>

#include <set>
#include <vector>

namespace Magic3D
{


class Model : public Resource
{
protected:
	std::vector<std::shared_ptr<Geometry>> meshes;
	std::shared_ptr<Geometry> collisionShape;
	std::shared_ptr<Material> material;

    mutable std::shared_ptr<CompoundGeometry> compound;
	
public:
	inline Model() {}

	inline Model(
        std::vector<std::shared_ptr<Geometry>>& meshes,
		std::shared_ptr<Material> material,
        std::shared_ptr<Geometry> collisionShape = nullptr
		): meshes(meshes), collisionShape(collisionShape), material(material) 
    {}

    inline Model(
        std::shared_ptr<Geometry> mesh,
        std::shared_ptr<Material> material,
        std::shared_ptr<Geometry> collisionShape = nullptr
        ) : collisionShape(collisionShape), material(material) 
    {
        if (mesh != nullptr)
            meshes.push_back(mesh);
    }

    inline std::vector<std::shared_ptr<Geometry>>& getMeshes()
	{
		return this->meshes;
	}
    inline void setMeshes(const std::vector<std::shared_ptr<Geometry>>& meshes)
	{
		this->meshes = meshes;
        this->compound = nullptr;
	}
    inline void setMeshes(std::shared_ptr<Geometry> mesh)
    {
        meshes.clear();
        meshes.push_back(mesh);
        this->compound = nullptr;
    }

    inline std::shared_ptr<Geometry> getCollisionShape()
	{
		return this->collisionShape;
	}
    inline void setCollisionShape(std::shared_ptr<Geometry> collisionShape)
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

    inline const Sphere& getGraphicalBoundingSphere() const
    {
        if (this->meshes.size() == 1)
            return this->meshes[0]->getBoundingSphere();

        if (this->compound != nullptr)
            return this->compound->getBoundingSphere();

        this->compound = std::make_shared<CompoundGeometry>(this->meshes);
        return this->compound->getBoundingSphere();
    }

    inline const Box& getGraphicalAABB() const
    {
        if (this->meshes.size() == 1)
            return this->meshes[0]->getAABB();

        if (this->compound != nullptr)
            return this->compound->getAABB();

        this->compound = std::make_shared<CompoundGeometry>(this->meshes);
        return this->compound->getAABB();
    }

};





};



#endif