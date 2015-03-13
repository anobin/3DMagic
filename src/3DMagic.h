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
#include "Util/Character.h"
#include "Util/StaticFont.h"

// math
#include "Math/Math.h"

// objects
#include "Objects/Object.h"

// graphics
#include "Graphics/Texture.h"
#include "Mesh/TriangleMeshBuilder.h"
#include "Graphics/MaterialBuilder.h"
#include "Graphics/Material.h"
#include "Lights/Light.h"

// time
#include "Time/StopWatch.h"

// resources
#include "Resources/Resource.h"
#include "Resources/TextResource.h"
#include "Resources/ResourceManager.h"
#include "Resources/fonts/TTFontResource.h"

// shaders
#include "Shaders\GpuProgram.h"

// physics
#include "Physics/MotionState.h"

// exceptions
#include "Exceptions/MagicException.h"
#include "Exceptions/ShaderCompileException.h"
#include "Exceptions/ResourceNotFoundException.h"
#include "Util/magic_assert.h"

// world and systems
#include "Graphics/GraphicsSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Event/EventSystem.h"
#include "World/World.h"


// cameras
#include "Cameras/FPCamera.h"
#include "Cameras/Camera2D.h"

// General Geometry API
#include <Geometry\Box.h>
#include <Geometry\Plane.h>
#include <Geometry\Sphere.h>
#include <Geometry\BoundedPlane.h>
#include <Geometry\ConvexHull.h>
#include <Geometry\CompoundGeometry.h>








#endif


