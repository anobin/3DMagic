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
#include <CollisionShapes\BoxCollisionShape.h>
#include <CollisionShapes\TriangleMeshCollisionShape.h>
#include <Graphics\Material.h>

#include <Geometry\Geometry.h>
#include <Geometry\Sphere.h>

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

    std::shared_ptr<SphereCollisionShape> graphicalBoundingSphere;
	
public:
	inline Model() {}

	inline Model(
        std::vector<std::shared_ptr<Geometry>>& meshes,
		std::shared_ptr<Material> material,
        std::shared_ptr<Geometry> collisionShape = nullptr
		): meshes(meshes), collisionShape(collisionShape), material(material) 
    {
        // TODO: fix this to build bounding sphere from all meshes
        TriangleMeshCollisionShape realShape(meshes[0]->getTriangleMesh());
        this->graphicalBoundingSphere = realShape.getBoundingSphere();
    }

    inline Model(
        std::shared_ptr<Geometry> mesh,
        std::shared_ptr<Material> material,
        std::shared_ptr<Geometry> collisionShape = nullptr
        ) : collisionShape(collisionShape), material(material) 
    {
        meshes.push_back(mesh);

        TriangleMeshCollisionShape realShape(mesh->getTriangleMesh());
        this->graphicalBoundingSphere = realShape.getBoundingSphere();
    }

    inline std::vector<std::shared_ptr<Geometry>>& getMeshes()
	{
		return this->meshes;
	}
    inline void setMeshes(const std::vector<std::shared_ptr<Geometry>>& meshes)
	{
		this->meshes = meshes;
        
        // TODO: fix this to build bounding sphere from all meshes
        TriangleMeshCollisionShape realShape(meshes[0]->getTriangleMesh());
        this->graphicalBoundingSphere = realShape.getBoundingSphere();
	}
    inline void setMeshes(std::shared_ptr<Geometry> mesh)
    {
        meshes.clear();
        meshes.push_back(mesh);

        TriangleMeshCollisionShape realShape(mesh->getTriangleMesh());
        this->graphicalBoundingSphere = realShape.getBoundingSphere();
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

    inline const SphereCollisionShape& getGraphicalBoundingSphere()
    {
        return *this->graphicalBoundingSphere;
    }

};





};



#endif