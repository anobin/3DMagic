
#include "ModelLoader3DS.h"
#include <string.h>
#include <lib3ds/mesh.h>
#include <Mesh/TriangleMesh.h>

namespace Magic3D
{


std::shared_ptr<Model> ModelLoader3DS::getModel(const std::string& path) const
{	
	Lib3dsFile* file = lib3ds_file_load(path.c_str());
	if (!file)
		throw_MagicException("Could not load model file");

    std::vector<std::shared_ptr<Geometry>> meshes;

    std::set<GpuProgram::AttributeType> attrs;
    attrs.insert(GpuProgram::AttributeType::VERTEX);
    attrs.insert(GpuProgram::AttributeType::TEX_COORD_0);
    attrs.insert(GpuProgram::AttributeType::NORMAL);
    attrs.insert(GpuProgram::AttributeType::TANGENT);

	// Loop through all the meshes
	int i;
	Lib3dsMesh * mesh;
	Vector3 p;
	for(mesh = file->meshes, i=0;mesh != NULL;mesh = mesh->next, i++)
	{
        MAGIC_THROW(mesh->points != mesh->texels,
            "Can only load meshes that have the same number of points and texels");

	    // allocate the batch
        auto batch = std::make_shared<TriangleMesh>(mesh->points, mesh->faces, attrs);
    
        for (unsigned int i = 0; i < mesh->points; i++)
        {
            auto vert = batch->getVertex<PositionAttr, TexCoordAttr>(i);
            vert.position(
                mesh->pointL[i].pos[0],
                mesh->pointL[i].pos[1],
                mesh->pointL[i].pos[2]
            );
            vert.texCoord(
                mesh->texelL[i][0],
                mesh->texelL[i][1]
            );
            batch->setVertex(i, vert);
        }

        // Loop through every face, setting the three vertices
        for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++)
        {
            Lib3dsFace* face = &mesh->faceL[cur_face];
            
            batch->setFace(cur_face, 
                TriangleMesh::Face(face->points[0], face->points[1], face->points[2]));
        }

        // TODO: add options on how these are done and thresholds
        batch->calculateNormalsAndTangents();
        
		meshes.push_back(batch);
	}

	lib3ds_file_free(file);

    auto model = std::make_shared<Model>();
    model->setMeshes(meshes);
    return model;
}
	
	
	
	
	
};