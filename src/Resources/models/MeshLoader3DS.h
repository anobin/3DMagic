/** Header file for Model3DSResource class
 *
 * @file Model3DSResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MODEL_3DS_RESOURCE_H
#define MAGIC3D_MODEL_3DS_RESOURCE_H

#include "../MeshLoader.h"
#include "../../Exceptions/MagicException.h"
#include <lib3ds/file.h>
#include "../../Math/Matrix4.h"

namespace Magic3D
{

/** Represents a single .3ds model resource
 */
class MeshLoader3DS : public MeshLoader
{
public:
	virtual std::shared_ptr<Meshes> getMeshes(const std::string& path) const;

};

};




#endif











