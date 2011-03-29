/* 
Copyright (c) 2011 Andrew Keating

This file is part of 3DMagic.

3DMagic is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

3DMagic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with 3DMagic.  If not, see <http://www.gnu.org/licenses/>.

*/
/** Master include file for 3DMagic library
 *
 * @file 3DMagic.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_H
#define MAGIC3D_H


// util
#include "Util/Color.h"
#include "Util/Units.h"

// math
#include "Math/Math.h"

// objects
#include "Objects/Object.h"

// graphics
#include "Graphics/Texture.h"

// time
#include "Time/StopWatch.h"

// resources
#include "Resources/Handle.h"
#include "Resources/Resource.h"
#include "Resources/ResourceManager.h"
#include "Resources/TextResource.h"
#include "Resources/Image2DResource.h"
#include "Resources/Images/TGA2DResource.h"
#include "Resources/ModelResource.h"
#include "Resources/models/Model3DSResource.h"

// shaders
#include "Shaders/Shader.h"
#include "Shaders/PointLightDiffuseShader.h"
#include "Shaders/FlatShader.h"
#include "Shaders/SimpleADSShader.h"
#include "Shaders/HemisphereShader.h"
#include "Shaders/HemisphereTexShader.h"
#include "Shaders/Shader2D.h"

// models
#include "Models/Model.h"
#include "Models/FlatSurface.h"
#include "Models/Sphere.h"
#include "Models/Box.h"
#include "Models/Rectangle2D.h"
#include "Models/Circle2D.h"
#include "Models/CustomModel.h"

// physics
#include "Physics/MotionState.h"

// exceptions
#include "Exceptions/MagicException.h"
#include "Exceptions/ShaderCompileException.h"
#include "Exceptions/ResourceNotFoundException.h"

















#endif


