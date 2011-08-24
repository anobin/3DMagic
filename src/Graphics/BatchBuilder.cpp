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
	    // make sure the full vertex count for each attribute was specified
	    MAGIC_THROW( buildData[i] != NULL && buildData[i]->currentVertex != this->vertexCount,
	        "Did not specify the full specified vertex count for a attribute" );
	    
	    // free all temporary build data
        delete buildData[i];
        buildData[i] = NULL;
    }
    
    // reset internal members
    this->batch = NULL;	
    this->vertexCount = 0;
    this->curAttributeCount = 0;
}
	






































	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};