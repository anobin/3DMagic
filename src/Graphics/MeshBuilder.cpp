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

#include <Graphics/MeshBuilder.h>
#include <Graphics/Mesh.h>

namespace Magic3D
{
	
/** Standard Constructor
 */
MeshBuilder::MeshBuilder(): mesh(NULL), vertexCount(0), curAttributeCount(0)
{
    for (int i=0; i < GpuProgram::MAX_ATTRIBUTE_TYPES; i++)
        buildData[i] = NULL;
}
	
/// destructor
MeshBuilder::~MeshBuilder()
{
	for (int i=0; i < GpuProgram::MAX_ATTRIBUTE_TYPES; i++)
        delete buildData[i];
}
	
/** Starts a vertex building sequence
 * @param vertexCount the number of verticies to be handled
 */
void MeshBuilder::begin(int vertexCount, int attributeCount, Mesh* mesh, 
						 VertexArray::Primitives primitive)
{
    MAGIC_THROW( this->mesh != NULL, "Called begin() before end() of previous build sequence." );
	MAGIC_THROW( vertexCount <= 0, "Invalid vertex count given to begin()");
	MAGIC_THROW( attributeCount <= 0 || attributeCount >= GpuProgram::MAX_ATTRIBUTE_TYPES, 
	    "Invalid attribute count given to begin()");
	
	// clear any build data
    for (int i=0; i < GpuProgram::MAX_ATTRIBUTE_TYPES; i++)
    {
        delete buildData[i];
        buildData[i] = NULL;
    }
    
    // init batch members, this also frees any current data
    mesh->allocate(vertexCount, attributeCount);
	mesh->primitive = primitive;
	
    // init internal members
	this->mesh = mesh;
	this->vertexCount = vertexCount;
	this->curAttributeCount = 0; // no current attributes
}

/** Modify a current batch.
 * @param batch the batch to modify
 */
void MeshBuilder::modify(Mesh* mesh)
{
	MAGIC_THROW(mesh->attributeData == NULL, "Tried to call modify() on a new batch, use begin().");
    MAGIC_THROW( this->mesh != NULL, "Called modify() before end() of previous build sequence." );
    
    // clear any current build data
    for (int i=0; i < GpuProgram::MAX_ATTRIBUTE_TYPES; i++)
    {
        delete buildData[i];
        buildData[i] = NULL;
    }
    
    // init build data to attributes in batch
    for(int i=0; i < mesh->attributeCount; i++)
    {
		Mesh::AttributeData& b = mesh->attributeData[i];
        buildData[(int)b.type] = new BuildData(&b);
    }
	
    // init internal members
	this->mesh = mesh;
	this->vertexCount = mesh->vertexCount;
	this->curAttributeCount = mesh->attributeCount;
}

/** end a vertex building sequence
 */
void MeshBuilder::end()
{
    // check to make sure all attributes were specified
    MAGIC_THROW(this->curAttributeCount != mesh->attributeCount, 
        "Failed to use the full number of specified attributes." );
   
    // go through all build data
	for (int i=0; i < GpuProgram::MAX_ATTRIBUTE_TYPES; i++)
	{   
	    // free all temporary build data
        delete buildData[i];
        buildData[i] = NULL;
    }

	/*this->mesh->vertexCount = this->batch->vertexCount;
	this->mesh->attributeCount = this->batch->attributeCount;
	this->mesh->primitive = this->batch->primitive;*/
    
    // reset internal members
	this->mesh = nullptr;
    this->vertexCount = 0;
    this->curAttributeCount = 0;
}
	
/** Manually set the current vertex for all currently 
 * know attributes. Note that any skipped over values are not set
 * to any specified value and that any new attributes set after this
 * will start at 0 as normal and not this vertex.
 * @param currentVertex the vertex to set
 */
void MeshBuilder::setCurrentVertex( int currentVertex )
{
    MAGIC_THROW( mesh == NULL, "Not currently in a build sequence." );
    MAGIC_THROW(currentVertex < 0 || currentVertex >= this->vertexCount,
        "Invalid current vertex to set." );
    
    // go through all build data
    for (int i=0; i < GpuProgram::MAX_ATTRIBUTE_TYPES; i++)
    {
        // set the current index for each valid attribute
        if (buildData[i] != NULL)
        {
            buildData[i]->setCurrentVertex(currentVertex,    
                GpuProgram::attributeTypeCompCount[i] );
        }
    }
}





































	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};