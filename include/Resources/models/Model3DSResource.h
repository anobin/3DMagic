/** Header file for Model3DSResource class
 *
 * @file Model3DSResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MODEL_3DS_RESOURCE_H
#define MAGIC3D_MODEL_3DS_RESOURCE_H

#include "../ModelResource.h"
#include "../../Exceptions/MagicException.h"
#include "../../Graphics/Batch.h"
#include <lib3ds/file.h>
#include "../../Math/Matrix4.h"

namespace Magic3D
{

/** Represents a single .3ds model resource
 */
class Model3DSResource : public ModelResource
{
protected:
    Lib3dsFile* file;
    
    int batchCount;

public:
	/** Standard constructor
	 * @param path the full path to the resource
	 * @param name the name for the resource
	 * @param manager the manager for the resource
	 */
	Model3DSResource(const char* path, const std::string& name, ResourceManager& manager);

	/// destructor
	virtual ~Model3DSResource();
	
	/** Get all the batches for this model resource
	 * @param batches array of batches, enough to accomidate batch count
	 */
	virtual void getAllBatches(Batch* batches, const Matrix4& transform = Matrix4()) const; 
	
	/** Get the number of batches in this model
	 * @return the number of batches in this model
	 */
	virtual int getBatchCount() const;


};

};




#endif











