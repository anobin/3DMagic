/** Header file for  ModelResource class
 * 
 * @file ModelResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MODEL_RESOURCE_H
#define MAGIC3D_MODEL_RESOURCE_H

#include "./Resource.h"
#include "../Graphics/Batch.h"

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
	/// destructor
	virtual ~ModelResource();
	
	
	/** Get all the batches for this model resource
	 * @param batches array of batches, enough to accomidate batch count
	 */
	virtual void getAllBatches(Batch* batches, float scale=1.0f) const = 0; 
	
	/** Get the number of batches in this model
	 * @return the number of batches in this model
	 */
	virtual int getBatchCount() const = 0;

};


};
















#endif













