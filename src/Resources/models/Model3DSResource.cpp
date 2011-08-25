/** Implementation file for Model3DSResource class
 * 
 * @file Model3DSResource.cpp
 * @author Andrew Keating
 */

#include <Resources/models/Model3DSResource.h>
#include <string.h>
#include <lib3ds/mesh.h>
#include <Graphics/BatchBuilder.h>

namespace Magic3D
{
	
/** Standard constructor
 * @param path the full path to the resource
 * @param name the name for the resource
 * @param manager the manager for the resource
 */
Model3DSResource::Model3DSResource(const char* path, const std::string& name, ResourceManager& manager):
	ModelResource(name, manager), file(NULL), batchCount(0)
{
	file = lib3ds_file_load(path);
	if (!file)
		throw_MagicException("Could not load model file");
	
	// count the number of meshes
	Lib3dsMesh * mesh;
	for(mesh = file->meshes, batchCount=0;
	    mesh != NULL;
	    mesh = mesh->next, batchCount++);
}

/// destructor
Model3DSResource::~Model3DSResource()
{
	lib3ds_file_free(file);
}
	
/** Get all the batches for this model resource
 * @param batches array of batches, enough to accomidate batch count
 */
void Model3DSResource::getAllBatches(Batch* batches, float scale) const
{	
	// Loop through all the meshes
	BatchBuilder bb;
	int i;
	Lib3dsMesh * mesh;
	for(mesh = file->meshes, i=0;mesh != NULL;mesh = mesh->next, i++)
	{
	    // start the batch
	    Batch* batch = &batches[i];
	    bb.begin((int)mesh->faces*3, 3, batch);
	    
	    // set normals
	    float* normals = new float[mesh->faces*3*3];
        lib3ds_mesh_calculate_normals(mesh, (float(*)[3])normals);
    
        // Loop through every face, setting the three vertices
        for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++)
        {
            Lib3dsFace * face = &mesh->faceL[cur_face];
            for(unsigned int j = 0;j < 3;j++)
            {   
                float* normal = &normals[cur_face*3*3 + j*3];
                bb.vertex3f( mesh->pointL[face->points[ j ]].pos[0] * scale, 
                    mesh->pointL[face->points[ j ]].pos[1] * scale, 
                    mesh->pointL[face->points[ j ]].pos[2] * scale 
                );
                bb.normal3f( normal[0], normal[1], normal[2] );
                bb.texCoord2f( 0.0f, 0.0f );
            }
        }
        
        // delete normals temporary storage
        delete normals;
        
        // end current batch
        bb.end();
	}
}

/** Get the number of batches in this model
 * @return the number of batches in this model
 */
int Model3DSResource::getBatchCount() const
{
    return batchCount;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};




