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
/** Implementation file for BatchBuilder class
 * 
 * @file BatchBuilder.cpp
 * @author Andrew Keating
 */

#include <Graphics/BatchBuilder.h>

namespace Magic3D
{
	
/** Standard Constructor
 */
BatchBuilder::BatchBuilder(): batch(NULL), vertexCount(0), curAttributeCount(0)
{
    for (int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
        buildData[i] = NULL;
}
	
/// destructor
BatchBuilder::~BatchBuilder()
{
	for (int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
        delete buildData[i];
}
	
/** Starts a vertex building sequence
 * @param vertexCount the number of verticies to be handled
 */
void BatchBuilder::begin(int vertexCount, int attributeCount, Batch* batch)
{
    MAGIC_THROW( this->batch != NULL, "Called begin() before end() of previous build sequence." );
	MAGIC_THROW( vertexCount <= 0, "Invalid vertex count given to begin()");
	MAGIC_THROW( attributeCount <= 0 || attributeCount >= Mesh::MAX_ATTRIBUTE_TYPES, 
	    "Invalid attribute count given to begin()");
	
	// clear any build data
    for (int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
    {
        delete buildData[i];
        buildData[i] = NULL;
    }
    
    // init batch members, this also frees any current data
    batch->allocate(vertexCount, attributeCount);
	
    // init internal members
	this->batch = batch;
	this->vertexCount = vertexCount;
	this->curAttributeCount = 0; // no current attributes
}

/** Modify a current batch.
 * @param batch the batch to modify
 */
void BatchBuilder::modify(Batch* batch)
{
    MAGIC_THROW(batch->data == NULL, "Tried to call modify() on a new batch, use begin()." );
    MAGIC_THROW( this->batch != NULL, "Called modify() before end() of previous build sequence." );
    
    // clear any current build data
    for (int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
    {
        delete buildData[i];
        buildData[i] = NULL;
    }
    
    // init build data to attributes in batch
    for(int i=0; i < batch->attributeCount; i++)
    {
        Batch::AttributeData& b = batch->data[i];
        buildData[(int)b.type] = new BuildData(&b);
    }
	
    // init internal members
	this->batch = batch;
	this->vertexCount = batch->vertexCount;
	this->curAttributeCount = batch->attributeCount;
}

/** end a vertex building sequence
 */
void BatchBuilder::end()
{
    // check to make sure all attributes were specified
    MAGIC_THROW(this->curAttributeCount != batch->attributeCount, 
        "Failed to use the full number of specified attributes." );
   
    // go through all build data
	for (int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
	{   
	    // free all temporary build data
        delete buildData[i];
        buildData[i] = NULL;
    }
    
    // reset internal members
    this->batch = NULL;	
    this->vertexCount = 0;
    this->curAttributeCount = 0;
}
	
/** Manually set the current vertex for all currently 
 * know attributes. Note that any skipped over values are not set
 * to any specified value and that any new attributes set after this
 * will start at 0 as normal and not this vertex.
 * @param currentVertex the vertex to set
 */
void BatchBuilder::setCurrentVertex( int currentVertex )
{
    MAGIC_THROW( batch == NULL, "Not currently in a build sequence." );
    MAGIC_THROW(currentVertex < 0 || currentVertex >= this->vertexCount,
        "Invalid current vertex to set." );
    
    // go through all build data
    for (int i=0; i < Mesh::MAX_ATTRIBUTE_TYPES; i++)
    {
        // set the current index for each valid attribute
        if (buildData[i] != NULL)
        {
            buildData[i]->setCurrentVertex(currentVertex,    
                Mesh::attributeTypeCompCount[i] );
        }
    }
}





































	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};