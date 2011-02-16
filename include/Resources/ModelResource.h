/** Header file for  ModelResource class
 * 
 * @file ModelResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MODEL_RESOURCE_H
#define MAGIC3D_MODEL_RESOURCE_H

#include "./Resource.h"
#include "../Graphics/VertexArray.h"

namespace Magic3D
{

/** Respresents a single model resource
 */
class ModelResource : public Resource
{
protected:
	/** Standard constructor
	 * @param name the name for this resource
	 * @param manager the manager for this resource
	 */
	inline ModelResource(const std::string& name, ResourceManager& manager): Resource(name, manager) {}


public:
	/// Data type for returned data arrays in main memory
	struct DataArray
	{
		/// number of components per array element
		int components;
		/// datatype of each component
		VertexArray::DataTypes dataType;
		/// data itself
		float* data;
	};
	
	/// destructor
	virtual ~ModelResource();
	
	
	/** Get data array for verticies of model
	 * @return the verticies of the model
	 */
	virtual const DataArray& getVertices() const = 0;
	
	/** Get data array for normals of model
	 * @return the normal vectors for the model
	 */
	virtual const DataArray& getNormals() const = 0;
	
	/** Get the texture coords for the model
	 * @return the texture coordinates of the model
	 */
	virtual const DataArray& getTextureCoords() const = 0;
	
	/** Get the number of vertices in this model
	 * @return the number of vertices in this model
	 */
	virtual int getVertexCount() const = 0;

	/** Clone this resource to get a copy allocated on the heap
	 * @return copy of this resource allocated on the heap
	 * @warning don't use this function unless you need to modify a
	 * resource
	 */
	virtual Resource* clone() const = 0;




};


};
















#endif













