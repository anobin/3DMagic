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
/** Header file for Buffer class 
 * 
 * @file Buffer.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_BUFFER_H
#define MAGIC3D_BUFFER_H

#include <glew.h>
#include <gl.h>
#include "MagicException.h"


namespace Magic3D
{
	
class VertexArray;

/** Abstraction for openGL buffers, represents
 * a segment of memory on the graphics card (most likely)
 * and the interface to the memory. Use of this class alongside
 * the glBindBuffer function can have unexpected results.
 */
class Buffer
{
public:
	/// types of buffer binding points
	enum BindingPoints
	{
		NO_BINDING = 0,
		ARRAY_BUFFER = GL_ARRAY_BUFFER,
		COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
		ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
		PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
		PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
		TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
		// the following were added in 3.1
		COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
		UNIFORM_BUFFER = GL_UNIFORM_BUFFER
	};
	
	friend class VertexArray;
	
private: // all static stuff to maintain buffer bindings go here
	// we maintain out own buffer bindings, cause openGL currently
	// offers no way to determine if a buffer is bound or what
	// point it is bound to
	/// global buffer bindings
	static GLuint bufferBindings[];
	
	/// check what point a buffer is currently bound to
	static inline BindingPoints getBindPoint(GLuint buffer)
	{
		for (int i=0; i < 9; i++)
		{
			if (buffer == bufferBindings[i])
			{
				switch(i)
				{
					case 0: return ARRAY_BUFFER;
					case 1: return COPY_WRITE_BUFFER;
					case 2: return ELEMENT_ARRAY_BUFFER;
					case 3: return PIXEL_PACK_BUFFER;
					case 4: return PIXEL_UNPACK_BUFFER;
					case 5: return TRANSFORM_FEEDBACK_BUFFER;
					case 6: return COPY_READ_BUFFER;
					case 7: return TEXTURE_BUFFER;
					case 8: return UNIFORM_BUFFER;
				}
			}
		}
		return NO_BINDING;
	}
	
	/// check what buffer is bound to a point
	static inline GLuint getBufferFromPoint(BindingPoints point)
	{
		switch (point)
		{
			case ARRAY_BUFFER: 				return bufferBindings[0];
			case COPY_WRITE_BUFFER:			return bufferBindings[1];
			case ELEMENT_ARRAY_BUFFER:		return bufferBindings[2];
			case PIXEL_PACK_BUFFER:			return bufferBindings[3];
			case PIXEL_UNPACK_BUFFER:		return bufferBindings[4];
			case TRANSFORM_FEEDBACK_BUFFER:	return bufferBindings[5];
			case COPY_READ_BUFFER:			return bufferBindings[6];
			case TEXTURE_BUFFER:			return bufferBindings[7];
			case UNIFORM_BUFFER:			return bufferBindings[8];
			default:
				return 0;
		}
	}
	
	/// bind a buffer
	static inline void bindBuffer(BindingPoints point, GLuint buffer)
	{
		switch (point)
		{
			case ARRAY_BUFFER: 				bufferBindings[0] = buffer; break;
			case COPY_WRITE_BUFFER:			bufferBindings[1] = buffer; break;
			case ELEMENT_ARRAY_BUFFER:		bufferBindings[2] = buffer; break;
			case PIXEL_PACK_BUFFER:			bufferBindings[3] = buffer; break;
			case PIXEL_UNPACK_BUFFER:		bufferBindings[4] = buffer; break;
			case TRANSFORM_FEEDBACK_BUFFER:	bufferBindings[5] = buffer; break;
			case COPY_READ_BUFFER:			bufferBindings[6] = buffer; break;
			case TEXTURE_BUFFER:			bufferBindings[7] = buffer; break;
			case UNIFORM_BUFFER:			bufferBindings[8] = buffer; break;
			default:
				throw MagicExceptionMacro("Tried to bind buffer to unknown point");
		}
		glBindBuffer(point, buffer);
	}
	
	/// unbind a buffer
	static inline void unBindBuffer(GLuint buffer)
	{
		BindingPoints point = getBindPoint(buffer);
		if (point != NO_BINDING)
			bindBuffer(point, 0);
	}
	

                	
private:
	/// openGL id for buffer
	GLuint bufferId;
	
public:
	/// the possible usage types for a buffer
	enum UsageTypes
	{
		// modified once, used infrequently, set by app, used by gl draw methods
		STREAM_DRAW = GL_STREAM_DRAW, 
		// modified once, used infrequently, set by gl, used to return to app
		STREAM_READ = GL_STREAM_READ, 
		// modified once, used infrequently, set by gl, used by gl draw methods
		STREAM_COPY = GL_STREAM_COPY, 
		// modified once, used often, set by app, used by gl draw methods
        STATIC_DRAW = GL_STATIC_DRAW, 
        // modified once, used often, set by gl, used to return to app
        STATIC_READ = GL_STATIC_READ,
        // modified once, used often, set by gl, used by gl draw methods
        STATIC_COPY = GL_STATIC_COPY,
        // modified often, used often, set by app, used by gl draw methods
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW, 
        // modified often, used often, set by gl, used to return to app
        DYNAMIC_READ = GL_DYNAMIC_READ,
        // modified often, used often, set by gl, used by gl draw methods
        DYNAMIC_COPY = GL_DYNAMIC_COPY
	};
	
	/// default constructor
	inline Buffer()
	{
		glGenBuffers(1, &bufferId);
	}
	
	/// constructor for specifying buffer size, but not contents
	inline Buffer(int size, UsageTypes usage)
	{
		glGenBuffers(1, &bufferId);
		allocate(size, NULL, usage);
	}
	
	/// constructor for specifying buffer size and contents
	inline Buffer(int size, const void* data, UsageTypes usage)
	{
		glGenBuffers(1, &bufferId);
		allocate(size, data, usage);
	}

	/// destructor
	inline ~Buffer()
	{
		Buffer::unBindBuffer(bufferId);
		glDeleteBuffers(1, &bufferId);
	}
	
	/** bind the buffer to a binding point to allow for
	 * other openGL functions to implicity use it
	 * @param point the binding point to bind to
	 */
	inline void bind(BindingPoints point)
	{
		Buffer::bindBuffer(point, bufferId);
	}
	
	/// unbind this buffer
	inline void unBind()
	{
		Buffer::unBindBuffer(bufferId);
	}
	
	/// get the current buffer binding
	inline BindingPoints getBinding()
	{
		return Buffer::getBindPoint(bufferId);
	}
	
	/** (re)allocate the buffer data
	 * @param size the size to allocate
	 * @param data the data to place in the buffer, can be NULL
	 * @param usage the planned usage of the buffer
	 */
	inline void allocate(int size, const void* data, UsageTypes usage)
	{
		// we use array buffer for no good reason, and we bypass static
		// functions becuase we restore previous buffer ourselves
		glBindBuffer(ARRAY_BUFFER, bufferId);
		glBufferData(ARRAY_BUFFER, size, data, usage);
		glBindBuffer(ARRAY_BUFFER, Buffer::getBufferFromPoint(ARRAY_BUFFER));
		
		if (glGetError() != GL_NO_ERROR)
			throw MagicExceptionMacro("Failed to allocate buffer");
	}
	
	/** fill an already allocated buffer
	 * @param offset the offset into the buffer to fill
	 * @param size the size of the data to copy
	 * @param data the data to copy
	 */
	inline void fill(int offset, int size, const void* data)
	{
		
		// we use array buffer for no good reason, and we bypass static
		// functions becuase we restore previous buffer ourselves
		glBindBuffer(ARRAY_BUFFER, bufferId);
		glBufferSubData(ARRAY_BUFFER, offset, size, data);
		glBindBuffer(ARRAY_BUFFER, Buffer::getBufferFromPoint(ARRAY_BUFFER));
		
		if (glGetError() != GL_NO_ERROR)
			throw MagicExceptionMacro("Failed to copy data");
	}






};


};


#endif









