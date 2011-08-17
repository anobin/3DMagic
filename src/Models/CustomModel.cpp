/** Implementation file for CustomModel class
 *
 * @file CustomModel.cpp
 * @author Andrew Keating
 */

#include <Models/CustomModel.h>
#include <Exceptions/MagicException.h>

#include <Graphics/VertexBatchBuilder.h>

namespace Magic3D
{
	
/** Standard constructor
 * @param resource the resource to make a model out of
 */
CustomModel::CustomModel(const ModelResource& resource)
{
	
	// get data arrays
	const ModelResource::DataArray& vertices = resource.getVertices();
	const ModelResource::DataArray& normals = resource.getNormals();
	const ModelResource::DataArray& tex = resource.getTextureCoords();
	
	// verify fit for now
	if (vertices.components != 3 || normals.components != 3 || tex.components != 2 ||
		vertices.dataType != VertexArray::FLOAT || normals.dataType != VertexArray::FLOAT ||
		tex.dataType != VertexArray::FLOAT)
	{
		throw_MagicException("model resource not compatiable");
	}
	
	int vertexCount = resource.getVertexCount();
	float* vertex = (float*)vertices.data;
	float* normal = (float*)normals.data;
	float* texel = (float*)tex.data;
	
	VertexBatchBuilder builder;
	builder.begin(vertexCount, &batch);
	
	// get indices for attributes we use
	int position_index = builder.getAttribId( "Position", 4, VertexArray::FLOAT );
	int normal_index = builder.getAttribId( "Normal", 3, VertexArray::FLOAT );
	int tex_index = builder.getAttribId( "TexCoord", 2, VertexArray::FLOAT );
	#define position3f(x, y, z) {builder.setAttribute4<float>(position_index, (x), (y), (z), 1.0f);}
	#define normal3f(x, y, z) {builder.setAttribute3<float>(normal_index, (x), (y), (z));}
	#define texCoord2f(x, y) {builder.setAttribute2<float>(tex_index, (x), (y));}
	
	for (int i=0,j=0,k=0; i < vertexCount; i++,j+=3,k+=2)
	{
		normal3f(normal[j], normal[j+1], normal[j+2]);
		texCoord2f(texel[k], texel[k+1]);
		position3f(vertex[j], vertex[j+1], vertex[j+2]);
	}
	builder.end();
	this->Model::data.push_back(&batch);
	
}


/// destructor
CustomModel::~CustomModel()
{
	/* intetentionally left blank */
}


/** Get the model's type name
 */
const char* CustomModel::getTypeName()
{
	return "Custom Model";
}
	
	
	
};
















