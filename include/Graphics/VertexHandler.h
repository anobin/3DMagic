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
/** Header file for VertexHandler class
 *
 * @file VertexHandler.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VERTEX_HANDLER_H
#define MAGIC3D_VERTEX_HANDLER_H

#include "../Exceptions/MagicException.h"
#include "../Shaders/ShaderVertexInterfaceSpec.h"
#include "../Exceptions/ShaderVertexInterfaceException.h"
#include "../Shaders/Shader.h"
#include "../Graphics/VertexArray.h"
#include "../Util/Color.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"

#include <map>

namespace Magic3D
{

/** Helps to manage vertex attributes when
 * building and drawing objects.
 */
class VertexHandler
{
private:
	/// data type for storage
	struct AttributeData
	{
		int index;
		Buffer* buffer;
		GLfloat* temp;
		int tempLength;
		int currentVertex;
		int components;
	};
	
	/// mapping of data pieces to data we use
	std::map<ShaderVertexInterfaceSpec::AttributeTypes, AttributeData*> data;
	
	/// vertex array that is handled
	VertexArray array;
	
	/// number of verticies being managed
	int vertexCount;
	
	/// default constructor
	inline VertexHandler(): vertexCount(0) {}
	
	inline void setAttribute4f(ShaderVertexInterfaceSpec::AttributeTypes type,
							   float c1, float c2, float c3, float c4)
	{
		std::map<ShaderVertexInterfaceSpec::AttributeTypes, AttributeData*>::iterator it
			= data.find(type);
		if (it == data.end())
			return;
		
		it->second->temp[it->second->currentVertex*it->second->components] = c1;
		it->second->temp[it->second->currentVertex*it->second->components+1] = c2;
		it->second->temp[it->second->currentVertex*it->second->components+2] = c3;
		it->second->temp[it->second->currentVertex*it->second->components+3] = c4;
		it->second->currentVertex += 1;
	}
	
	inline void setAttribute3f(ShaderVertexInterfaceSpec::AttributeTypes type,
							   float c1, float c2, float c3)
	{
		std::map<ShaderVertexInterfaceSpec::AttributeTypes, AttributeData*>::iterator it
			= data.find(type);
		if (it == data.end())
			return;
		
		it->second->temp[it->second->currentVertex*it->second->components] = c1;
		it->second->temp[it->second->currentVertex*it->second->components+1] = c2;
		it->second->temp[it->second->currentVertex*it->second->components+2] = c3;
		it->second->currentVertex += 1;
	}
	
	inline void setAttribute2f(ShaderVertexInterfaceSpec::AttributeTypes type,
							   float c1, float c2)
	{
		std::map<ShaderVertexInterfaceSpec::AttributeTypes, AttributeData*>::iterator it
			= data.find(type);
		if (it == data.end())
			return;
		
		it->second->temp[it->second->currentVertex*it->second->components] = c1;
		it->second->temp[it->second->currentVertex*it->second->components+1] = c2;
		it->second->currentVertex += 1;
	}

public:
	/** Standard Constructor
	 * @param spec the shader-vertex interface spec to use
	 */
	VertexHandler(const ShaderVertexInterfaceSpec& spec);
			
	/** Standard Constructor for specifying multiple shaders
	 * that need to have their requirements meet
	 * @param specCount the number of specs provided
	 * @param ... const references to the shader-vertex interface specs to use
	 */
	VertexHandler(int specCount, ...);
	
	/// destructor
	~VertexHandler();
	
	/** Starts a vertex building sequence
	 * @param vertexCount the number of verticies to be handled
	 */
	void begin(int vertexCount);
	
	
	/// specify new 3 component position coord
	inline void position3f(float x, float y, float z)
	{
		this->position4f(x, y, z, 1.0f);
	}
	/// specify a new 4 component position coord
	inline void position4f(float x, float y, float z, float w)
	{
		this->setAttribute4f(ShaderVertexInterfaceSpec::POSITION, x, y, z, w);
	}
	/// specify a new position
	inline void position(const Point3f& point)
	{
		this->position4f(point.getX(), point.getY(), point.getZ(), 1.0f);
	}
	/// specify a new position
	inline void position(const Point4f& point)
	{
		this->setAttribute4f(ShaderVertexInterfaceSpec::POSITION, 
							 point.getX(), point.getY(), point.getZ(), point.getW());
	}
	
		
	/// specify a new normal vector
	inline void normal3f(float x, float y, float z)
	{
		this->setAttribute3f(ShaderVertexInterfaceSpec::NORMAL, x, y, z);
	}
	/// specify a new normal vector
	inline void normal(const Vector3f& vector)
	{
		this->setAttribute3f(ShaderVertexInterfaceSpec::NORMAL, vector.getX(), 
							 vector.getY(), vector.getZ());
	}
	
	
	/// specify a new color component
	inline void color4f(float r, float g, float b, float a)
	{
		this->setAttribute4f(ShaderVertexInterfaceSpec::COLOR, r, g, b, a);
	}
	inline void color(const Color& color)
	{
		const GLfloat* d = color.getInternal();
		this->setAttribute4f(ShaderVertexInterfaceSpec::COLOR, d[0], d[1], d[2], d[3]);
	}
	
	
	/// specify a new texture coord
	inline void texCoord2f(float x, float y)
	{
		this->setAttribute2f(ShaderVertexInterfaceSpec::BASE_TEXTURE, x, y);
	}
	inline void texCoord(const Point2f& point)
	{
		this->setAttribute2f(ShaderVertexInterfaceSpec::BASE_TEXTURE, point.getX(), 
							 point.getY());
	}
	
	
	/** end a vertex building sequence
	 */
	void end();
	
	/// draw all verticies
	inline void draw(VertexArray::Primitives primitive = VertexArray::TRIANGLES)
	{
		array.draw(primitive, vertexCount);
	}










};


};




#endif
















