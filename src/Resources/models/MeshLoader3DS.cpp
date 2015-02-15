
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
    MeshBuilderPTN bb;
	int i;
	Lib3dsMesh * mesh;
	Point3 p;
	for(mesh = file->meshes, i=0;mesh != NULL;mesh = mesh->next, i++)
	{
	    // start the batch
		bb.reset((int)mesh->faces*3);
	    
	    // get normals
	    float* normals = new float[mesh->faces*3*3];
        lib3ds_mesh_calculate_normals(mesh, (float(*)[3])normals);
    
        // Loop through every face, setting the three vertices
        for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++)
        {
            Lib3dsFace * face = &mesh->faceL[cur_face];
            for(unsigned int j = 0;j < 3;j++)
            { 

                bb.addVertex(
                    Vector3(
                        mesh->pointL[face->points[j]].pos[0],
                        mesh->pointL[face->points[j]].pos[1],
                        mesh->pointL[face->points[j]].pos[2]
                    ),
                    Vector2(0.0f, 0.0f), // TODO: set correct tex coords
                    Vector3(
                        normals[(cur_face*3*3)+(j*3)+0],
                        normals[(cur_face*3*3)+(j*3)+1],
                        normals[(cur_face*3*3)+(j*3)+2]
                    )
                );
            }
        }

        delete normals;
        
        // end current mesh
		meshes->push_back(bb.build());
	}

	lib3ds_file_free(file);

	return meshes;
}
	
	
	
	
	
};