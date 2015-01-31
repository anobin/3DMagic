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
    MeshBuilder bb;
    float temp[3][3];
    
    // go through all batches and add all vertices
    // and add triangles to physics mesh
    for(auto batch : batches)
    {
		bb.modify(batch.get());
        for(int j=0; j < batch->getVertexCount(); j+=3)
        {
            // get three vertices for triangle
            bb.setCurrentVertex(j);
            bb.getVertex3f( &temp[0][0], &temp[0][1], &temp[0][2] );
            bb.setCurrentVertex(j+1);
            bb.getVertex3f( &temp[1][0], &temp[1][1], &temp[1][2] );
            bb.setCurrentVertex(j+2);
            bb.getVertex3f( &temp[2][0], &temp[2][1], &temp[2][2] );
            
            // add triangle to mesh
            mesh.addTriangle( btVector3(temp[0][0], temp[0][1], temp[0][2]),
                btVector3(temp[1][0], temp[1][1], temp[1][2]),
                btVector3(temp[2][0], temp[2][1], temp[2][2]), true
            );
        }
        bb.end();
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










