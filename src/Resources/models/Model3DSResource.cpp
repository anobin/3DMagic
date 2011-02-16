/** Implementation file for Model3DSResource class
 * 
 * @file Model3DSResource.cpp
 * @author Andrew Keating
 */

#include <Resources/models/Model3DSResource.h>
#include <string.h>
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>

namespace Magic3D
{
	
/** Standard constructor
 * @param path the full path to the resource
 * @param name the name for the resource
 * @param manager the manager for the resource
 */
Model3DSResource::Model3DSResource(const char* path, const std::string& name, ResourceManager& manager):
	ModelResource(name, manager)
{
	Lib3dsFile* file = lib3ds_file_load(path);
	if (!file)
		throw MagicExceptionMacro("Could not load model file");
	
	// count all the vertices
	vertexCount = 0;
	Lib3dsMesh * mesh;
	for(mesh = file->meshes;mesh != NULL;mesh = mesh->next)
		vertexCount += mesh->faces;
	vertexCount *= 3;
	
	// allocate data arrays
	vertices.components = 3;
	vertices.dataType = VertexArray::FLOAT;
	vertices.data = new float[vertexCount*3];
	normals.components = 3;
	normals.dataType = VertexArray::FLOAT;
	normals.data = new float[vertexCount*3];
	texels.components = 2;
	texels.dataType = VertexArray::FLOAT;
	texels.data = new float[vertexCount*2];
	
	// load vertex attributes into data arrays
	int doneFaces = 0;
	// Loop through all the meshes
	for(mesh = file->meshes;mesh != NULL;mesh = mesh->next)
	{
			lib3ds_mesh_calculate_normals(mesh, (float(*)[3])&normals.data[doneFaces*3*3]);
		
			// Loop through every face
			for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++)
			{
					Lib3dsFace * face = &mesh->faceL[cur_face];
					for(unsigned int i = 0;i < 3;i++)
					{
							memcpy(&vertices.data[doneFaces*3*3 + i*3], mesh->pointL[face->points[ i ]].pos, sizeof(float)*3);
							//texels.data[FinishedFaces*3*2 + i*2] = 0.0f;
							//texels.data[FinishedFaces*3*2 + i*2 + 1] = 0.0f;
					}
					doneFaces++;
			}
	}
	
	
	lib3ds_file_free(file);
}

/// destructor
Model3DSResource::~Model3DSResource()
{
	delete[] vertices.data;
	delete[] normals.data;
	delete[] texels.data;
}
	
/** Get data array for verticies of model
 * @return the verticies of the model
 */
const ModelResource::DataArray& Model3DSResource::getVertices() const
{
	return this->vertices;
}
	
/** Get data array for normals of model
 * @return the normal vectors for the model
 */
const ModelResource::DataArray& Model3DSResource::getNormals() const
{
	return this->normals;
}
	
/** Get the texture coords for the model
 * @return the texture coordinates of the model
 */
const ModelResource::DataArray& Model3DSResource::getTextureCoords() const
{
	return this->texels;
}
	
/** Get the number of vertices in this model
 * @return the number of vertices in this model
 */
int Model3DSResource::getVertexCount() const
{
	return this->vertexCount;
}

/** Clone this resource to get a copy allocated on the heap
 * @return copy of this resource allocated on the heap
 * @warning don't use this function unless you need to modify a
 * resource
 */
Resource* Model3DSResource::clone() const
{
	return new Model3DSResource(*this);
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};




