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
/** Implementation file for TextResource class
 *
 * @file TextResource.cpp
 * @author Andrew Keating
 */

#include <Resources/TextResource.h>
#include <Exceptions/ResourceNotFoundException.h>
#include <iostream>
#include <fstream>

namespace Magic3D
{
	
/** Standard constructor
 * @param path full path for text file to read
 */
TextResource::TextResource(const char* path, const std::string& name, ResourceManager& manager): Resource(name, manager)
{
	std::ifstream file;
	file.open (path, std::ios::binary );
	
	if (!file.is_open() || !file.good())
	{
		file.close();
		throw MagicException("Failed to open text resource");
	}

	// get length of file:
	file.seekg (0, std::ios::end);
	int length = (int)file.tellg();
	file.seekg (0, std::ios::beg);

	// read all data into memory
	text = new char [length+1];
	file.read (text,length);
	text[length] = 0; // null terminated string
  
	// close file
	file.close();
}

/// destructor
TextResource::~TextResource()
{
	delete[] text;
}
	
/** Retrieve the text data for this resource
 * @return the text data as a const C string
 */
const char* TextResource::getText() const
{
	return this->text;
}

/** Clone this resource to get a copy allocated on the heap
 * @return copy of this resource allocated on the heap
 */
Resource* TextResource::clone() const
{
	return new TextResource(*this);
}
	
	
	
	
	
	
	
	
	
	
	
	
	
};








