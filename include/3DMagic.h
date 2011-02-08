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


// basic classes
#include "Matrix.h"
#include "MagicException.h"
#include "Color.h"
#include "Units.h"
#include "Vector.h"
#include "Point.h"
#include "Object.h"
#include "Position.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Handle.h"
#include "Texture.h"
#include "Model.h"

// time
#include "Time/StopWatch.h"

// resources
#include "Resources/TextResource.h"
#include "Resources/Image2DResource.h"
#include "Resources/Images/TGA2DResource.h"

// shaders
#include "Shaders/PointLightDiffuseShader.h"
#include "Shaders/FlatShader.h"
#include "Shaders/SimpleADSShader.h"
#include "Shaders/HemisphereShader.h"
#include "Shaders/HemisphereTexShader.h"

// models
#include "Models/FlatSurface.h"
#include "Models/Sphere.h"
#include "Models/Box.h"

// physics
#include "Physics/MotionState.h"

// exceptions
#include "Exceptions/ShaderCompileException.h"
#include "Exceptions/ResourceNotFoundException.h"

















#endif


