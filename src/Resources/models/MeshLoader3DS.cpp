
#include "MeshLoader3DS.h"
#include <string.h>
#include <lib3ds/mesh.h>
#include <Graphics/MeshBuilder.h>
#include <Math/Point.h>

namespace Magic3D
{


std::shared_ptr<Meshes> MeshLoader3DS::getMeshes(const std::string& path) const
{	
	Lib3dsFile* file = lib3ds_file_load(path.c_str());
	if (!file)
		throw_MagicException("Could not load model file");

	std::shared_ptr<Meshes> meshes = std::make_shared<Meshes>();

	// Loop through all the meshes
	MeshBuilder bb;
	int i;
	Lib3dsMesh * mesh;
	Point3 p;
	for(mesh = file->meshes, i=0;mesh != NULL;mesh = mesh->next, i++)
	{
	    // start the batch
		std::shared_ptr<Mesh> batch = std::make_shared<Mesh>();
		bb.begin((int)mesh->faces*3, 3, batch.get());
	    
	    // set normals
	    float* normals = new float[mesh->faces*3*3];
        lib3ds_mesh_calculate_normals(mesh, (float(*)[3])normals);
        bb.normal3fv(normals, mesh->faces*3);
        delete normals;
    
        // Loop through every face, setting the three vertices
        for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++)
        {
            Lib3dsFace * face = &mesh->faceL[cur_face];
            for(unsigned int j = 0;j < 3;j++)
            {   
                p = Point3(
					mesh->pointL[face->points[ j ]].pos[0],
					mesh->pointL[face->points[ j ]].pos[1],
					mesh->pointL[face->points[ j ]].pos[2]
				);
                bb.vertex3f( p.x(), p.y(), p.z() );
                bb.texCoord2f( 0.0f, 0.0f );
            }
        }
        
        // end current batch
        bb.end();
		meshes->push_back(batch);
	}

	lib3ds_file_free(file);

	return meshes;
}
	
	
	
	
	
};