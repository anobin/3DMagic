
#include "ModelLoader3DS.h"
#include <string.h>
#include <lib3ds/mesh.h>
#include <Mesh/TriangleMeshBuilder.h>

namespace Magic3D
{


std::shared_ptr<Model> ModelLoader3DS::getModel(const std::string& path) const
{	
	Lib3dsFile* file = lib3ds_file_load(path.c_str());
	if (!file)
		throw_MagicException("Could not load model file");

    std::vector<std::shared_ptr<TriangleMesh>> meshes;

	// Loop through all the meshes
    TriangleMeshBuilderPTNT bb(10);
	int i;
	Lib3dsMesh * mesh;
	Vector3 p;
	for(mesh = file->meshes, i=0;mesh != NULL;mesh = mesh->next, i++)
	{
        MAGIC_THROW(mesh->points != mesh->texels,
            "Can only load meshes that have the same number of points and texels");

	    // start the batch
		bb.reset(mesh->points, mesh->faces);
    
        for (unsigned int i = 0; i < mesh->points; i++)
        {

            bb.addVertex(
                Vector3(
                    mesh->pointL[i].pos[0],
                    mesh->pointL[i].pos[1],
                    mesh->pointL[i].pos[2]
                ),
                Vector2(
                    mesh->texelL[i][0],
                    mesh->texelL[i][1]
                )
            );
        }

        // Loop through every face, setting the three vertices
        for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++)
        {
            Lib3dsFace* face = &mesh->faceL[cur_face];
            
            bb.addFace(face->points[0], face->points[1], face->points[2]);
        }

        bb.calculateDuplicateVertices();
        bb.calculateNormalsAndTangents();
        
        // end current mesh
		meshes.push_back(bb.build());
	}

	lib3ds_file_free(file);

    auto model = std::make_shared<Model>();
    model->setMeshes(meshes);
    return model;
}
	
	
	
	
	
};