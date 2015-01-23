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
/** Header file for TextResource class
 *
 * @file TextResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_TEXT_RESOURCE_H
#define MAGIC3D_TEXT_RESOURCE_H

#include "Resource.h"
#include <string>

namespace Magic3D
{
	
/** Respresents a single text resource
 */
class TextResource : public Resource
{
protected:
	/// text data read in from file
	char* text;

public:
	/** Standard constructor
	 * @param path full path for text file to read
	 */
	TextResource(const std::string& path, const std::string& name);

	/// copy constructor
	inline TextResource(const TextResource& copy): Resource(copy.name), text(copy.text) {}
	
	/// destructor
	virtual ~TextResource();
	
	
	/** Retrieve the text data for this resource
	 * @return the text data as a const C string
	 */
	virtual const char* getText() const;
	
	
	
	
};	
	
	
	
};



#endif



