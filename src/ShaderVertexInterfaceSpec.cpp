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
/** Implementation file for ShaderVertexInterfaceSpec class
 *
 * @param ShaderVertexInterfaceSpec.cpp
 * @author Andrew Keating
 */

#include <ShaderVertexInterfaceSpec.h>

namespace Magic3D
{

// default interfaces, god that class name is long
	
/// default interface for just a position
ShaderVertexInterfaceSpec ShaderVertexInterfaceSpec::DefaultSpec_Position
	(1, 0, ShaderVertexInterfaceSpec::POSITION);
	
/// default interface for a position and normal
ShaderVertexInterfaceSpec ShaderVertexInterfaceSpec::DefaultSpec_Position_Normal
	(2, 0, ShaderVertexInterfaceSpec::POSITION, 1, ShaderVertexInterfaceSpec::NORMAL);

/// default interface for a position and base texture
ShaderVertexInterfaceSpec ShaderVertexInterfaceSpec::DefaultSpec_Position_Texture
	(2, 0, ShaderVertexInterfaceSpec::POSITION, 3, ShaderVertexInterfaceSpec::BASE_TEXTURE);

/// default interface for a position, normal, and base texture
ShaderVertexInterfaceSpec ShaderVertexInterfaceSpec::DefaultSpec_Position_Normal_Texture
	(3, 0, ShaderVertexInterfaceSpec::POSITION, 1, ShaderVertexInterfaceSpec::NORMAL,
		3, ShaderVertexInterfaceSpec::BASE_TEXTURE);
	
	
	
	
	
	
	
	
	
	
	
	
	
};