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
/** Header file for VertexArray class 
 *
 * @file VertexArray.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VERTEX_ARRAY_H
#define MAGIC3D_VERTEX_ARRAY_H

#ifdef _WIN32
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <glew.h>
#include <gl.h>
#endif

#include "Buffer.h"
#include "../Exceptions/MagicException.h"

namespace Magic3D
{

/** Abstraction of opengl vertex arrays, allows for
 * access to segment of memory that stores the per-vertex
 * attributes that are used by the vertex shader during
 * a drawing operation
 */
class VertexArray
{
private:
	/// opengl id of this vertex array
	GLuint arrayId;
	
	/// used to ensure we don't unbind someone else's vertex array
	static GLuint boundArrayId;


public:
	/// acceptable data types for attribute arrays
	enum DataTypes
	{
		BYTE = GL_BYTE,
		UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
		SHORT = GL_SHORT,
		UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
		INT = GL_INT,
		UNSIGNED_INT = GL_UNSIGNED_INT,
		HALF_FLOAT = GL_HALF_FLOAT,
		FLOAT = GL_FLOAT,
		DOUBLE = GL_DOUBLE,
		INT_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
		UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV
	};
	
	/// types of primitives
	enum Primitives
	{
		POINTS = GL_POINTS,
		LINE_STRIP = GL_LINE_STRIP,
		LINE_LOOP = GL_LINE_LOOP,
		LINES = GL_LINES,
		LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
		LINES_ADJACENCY = GL_LINES_ADJACENCY,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		TRIANGLE_FAN = GL_TRIANGLE_FAN,
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
		TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY
	};
	
	/// default constructor
	inline VertexArray()
	{
#ifndef MAGIC3D_NO_VERTEX_ARRAYS
		glGenVertexArrays(1, &arrayId); //openGL 3
#endif
	}
	
	/// destructor
	inline ~VertexArray()
	{
		unBind();
#ifndef MAGIC3D_NO_VERTEX_ARRAYS
		glDeleteVertexArrays(1, &arrayId); //openGL 3
#endif
	}
	
	/// bind this vertex array 
	inline void bind()
	{
#ifndef MAGIC3D_NO_VERTEX_ARRAYS
		VertexArray::boundArrayId = arrayId;
		glBindVertexArray(arrayId); //openGL 3
#endif
	}
	
	/// unbind this vertex array
	inline void unBind()
	{
#ifndef MAGIC3D_NO_VERTEX_ARRAYS
		if (arrayId != VertexArray::boundArrayId)
			return;
		glBindVertexArray(0); // openGL 3
		VertexArray::boundArrayId = 0;
#endif
	}
	
	/** set an attribute array of the vertex array
	 * @param index the attribute index to set
	 * @param components the number of components per vertex (vector size in shader)
	 * @param type data type/size for each component
	 * @param buffer the buffer to be used as the attribute array
	 */
	inline void setAttributeArray(unsigned int index, int components, DataTypes type, 
								  Buffer& buffer)
	{
		this->bind();
		glEnableVertexAttribArray(index);
		buffer.bind(Buffer::ARRAY_BUFFER);
		glVertexAttribPointer(index, 		// attribute index
							  components,   // number of components per vertex
							  type, 		// the data type of each component
							  GL_FALSE, 	// no normalizing
							  0, 			// no padding
							  0				// no offset to start at
							 );
		buffer.unBind();
		this->unBind();
		
		if (glGetError() != GL_NO_ERROR)
			throw MagicExceptionMacro("Failed to set attribute array");
	}
	
	/// disable an attribute array from being used
	inline void disableAttributeArray(unsigned int index)
	{
		this->bind();
		glDisableVertexAttribArray(index);
		this->unBind();
	}
	 
	/** render a number of verticies using this vertex array as the 
	 * data for each vertex
	 * @param primitive the type of the primitives to draw
	 * @param vertexCount the number of verticies to render
	 * @param startingVertex the vertex to start at
	 */
	inline void draw(Primitives primitive, unsigned int vertexCount, unsigned int startingVertex = 0)
	{
		this->bind();
		glDrawArrays(primitive, startingVertex, vertexCount);
		this->unBind();
		if (glGetError() != GL_NO_ERROR)
			throw MagicExceptionMacro("Failed to draw");
	}


};


};


#endif







