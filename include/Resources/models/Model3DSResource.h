/** Header file for Model3DSResource class
 *
 * @file Model3DSResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MODEL_3DS_RESOURCE_H
#define MAGIC3D_MODEL_3DS_RESOURCE_H

#include "../ModelResource.h"
#include "../../Exceptions/MagicException.h"

namespace Magic3D
{

/** Represents a single .3ds model resource
 */
class Model3DSResource : public ModelResource
{
protected:
	// data arrays needed
	DataArray vertices;
	DataArray normals;
	DataArray texels;
	int vertexCount;

public:
	/** Standard constructor
	 * @param path the full path to the resource
	 * @param name the name for the resource
	 * @param manager the manager for the resource
	 */
	Model3DSResource(const char* path, const std::string& name, ResourceManager& manager);

	/// destructor
	virtual ~Model3DSResource();
	
	/// copy constructor
	Model3DSResource(const Model3DSResource& copy): ModelResource(copy.name, *copy.manager)
	{
		throw MagicExceptionMacro("i'm too tired to make this right now");
	}
	
	/** Get data array for verticies of model
	 * @return the verticies of the model
	 */
	virtual const DataArray& getVertices() const;
	
	/** Get data array for normals of model
	 * @return the normal vectors for the model
	 */
	virtual const DataArray& getNormals() const;
	
	/** Get the texture coords for the model
	 * @return the texture coordinates of the model
	 */
	virtual const DataArray& getTextureCoords() const;
	
	/** Get the number of vertices in this model
	 * @return the number of vertices in this model
	 */
	virtual int getVertexCount() const;

	/** Clone this resource to get a copy allocated on the heap
	 * @return copy of this resource allocated on the heap
	 * @warning don't use this function unless you need to modify a
	 * resource
	 */
	virtual Resource* clone() const;



};

};




#endif











