/** Header file for Model3DSResource class
 *
 * @file Model3DSResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MODEL_3DS_RESOURCE_H
#define MAGIC3D_MODEL_3DS_RESOURCE_H

#include "../BatchLoader.h"
#include "../../Exceptions/MagicException.h"
#include "../../Graphics/Batch.h"
#include <lib3ds/file.h>
#include "../../Math/Matrix4.h"

namespace Magic3D
{

/** Represents a single .3ds model resource
 */
class BatchLoader3DS : public BatchLoader
{
public:
	virtual std::shared_ptr<Batches> getBatches(const std::string& path) const;

};

};




#endif











