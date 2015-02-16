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
/** Implementation file for TriangleMeshCollisionShape class
 *
 * @file TriangleMeshCollisionShape.cpp
 * @author Andrew Keating
 */
 
#include <CollisionShapes/TriangleMeshCollisionShape.h>
#include <Graphics/MeshBuilder.h>

namespace Magic3D
{
    
/// default constructor
TriangleMeshCollisionShape::TriangleMeshCollisionShape(Meshes& batches):
    shape(NULL)
{    
    // go through all batches and add all vertices
    // and add triangles to physics mesh
    for(auto mesh : batches)
    {
        for(int j=0; j < mesh->getVertexCount(); j+=3)
        {
            // get three vertices for triangle
            auto a = mesh->getVertex(j).position();
            auto b = mesh->getVertex(j+1).position();
            auto c = mesh->getVertex(j+2).position();
            
            // add triangle to mesh
            this->mesh.addTriangle( btVector3(a.x(), a.y(), a.z()),
                btVector3(b.x(), b.y(), b.z()),
                btVector3(c.x(), c.y(), c.z()), true
            );
        }
    }
    
    // build physics shape from mesh data
    this->shape = new btBvhTriangleMeshShape(&mesh, true);
}
  
/// destructor
TriangleMeshCollisionShape::~TriangleMeshCollisionShape()
{
    delete shape;
}   
    
    
/// get the bullet physics collison shape
btCollisionShape* TriangleMeshCollisionShape::getShape()
{
    return this->shape;
}      
    
    
    
    
    
};










