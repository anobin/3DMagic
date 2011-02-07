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
/** Header file for ShaderVertexInterfaceSpec class
 *
 * @file ShaderVertexInterfaceSpec.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_SHADER_VERTEX_INTERFACE_H
#define MAGIC3D_SHADER_VERTEX_INTERFACE_H

#include <map>
#include <stdarg.h>
#include <MagicException.h>

namespace Magic3D
{


/** Represents the specification of the interface between
 * the openGL code and the vertex shader in regards to 
 * vertex attributes (what data should go into which attribute
 * index and what data is needed). Uniform data is set through
 * methods on the shader class itself.
 */
class ShaderVertexInterfaceSpec
{
public:
	/// data pieces allowed to be attributes
	enum AttributeTypes
	{
		POSITION, 		//vec4
		NORMAL,			//vec3
		COLOR,			//vec4
		BASE_TEXTURE	//vec2
	};
	
private:
	/// mapping of indexes and data expected
	std::map<int, AttributeTypes> attributes;
	
	/// default constructor
	inline ShaderVertexInterfaceSpec() {}
	
public:
	/** Standard constructor specifying attributed with
	 * an index followed by an attribute
	 * @param attributeCount the number of attributes
	 */
	inline ShaderVertexInterfaceSpec(int attributeCount, ...)
	{
		va_list attributeList;
		va_start(attributeList, attributeCount);
		for(int i = 0; i < attributeCount; i++)
		{
			int index = va_arg(attributeList, int); // index of attribute
			AttributeTypes type = (AttributeTypes)va_arg(attributeList, int); // type of attribute
			attributes.insert(std::pair<int, AttributeTypes>(index, type));
		}
		va_end(attributeList);
	}
	
	/// destructor
	inline ~ShaderVertexInterfaceSpec()
	{
		// inline to avoid children
	}

	/// get the attribute mappings
	inline const std::map<int, AttributeTypes>& getAttributeMappings() const
	{
		return this->attributes;
	}


	/* a number of default interfaces are given that are all
	 * compatable with each other, use these if possible to allow
	 * for easy shader data requirements combination for static vertex handler.
	 * The default mapping is
	 * 0 -> POSITION
	 * 1 -> NORMAL
	 * 2 -> COLOR
	 * 3 -> BASE_TEXTURE
	 */
	
	/// get the default index for a data piece
	inline static int getDefaultMapping(AttributeTypes type)
	{
		switch (type)
		{
			case POSITION:		return 0;
			case NORMAL:		return 1;
			case COLOR:			return 2;
			case BASE_TEXTURE:	return 3;
			default:
				throw MagicExceptionMacro("Attempted to get default mapping for a"
					" attribute data piece that does not have a default mapping");
		}
	}
	
	/// default interface for just a position
	static ShaderVertexInterfaceSpec DefaultSpec_Position;
	
	/// default interface for a position and normal
	static ShaderVertexInterfaceSpec DefaultSpec_Position_Normal;
	
	/// default interface for a position and base texture
	static ShaderVertexInterfaceSpec DefaultSpec_Position_Texture;
	
	/// default interface for a position, normal, and base texture
	static ShaderVertexInterfaceSpec DefaultSpec_Position_Normal_Texture;


};


};




#endif




















