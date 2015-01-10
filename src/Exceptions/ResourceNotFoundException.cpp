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
/** Implementation file for ResourceNotFoundException class
 *
 * @file ResourceNotFoundException.cpp
 * @author Andrew Keating
 */

#include <Exceptions/ResourceNotFoundException.h>

#include <sstream>

namespace Magic3D
{

/** Standard constructor
 * @param resource the resource that cannot be found
 * @param file the file the error occurred in
 * @param line the line of the file where the error occurred
 */
ResourceNotFoundException::ResourceNotFoundException(const std::string& resource, const char* file, int line): 
	missingResource(resource)
{
	std::ostringstream s;
	s << "(" << file << ":" << line << ") " << "Resource \"" << resource << "\" was not found";
	this->MagicException::error = s.str();
}
	
/// destructor
ResourceNotFoundException::~ResourceNotFoundException() throw()
{
	/* intentionally left blank */
}
	
/** Retrieve error message
 * @return the error message
 */
const char* ResourceNotFoundException::what() const throw()
{
	return this->MagicException::error.c_str();
}
	
	
	
/** Retrieve the resource that could not be found
 * @return name of the resource that could not be found
 */
const char* ResourceNotFoundException::getMissingResourceName() const throw()
{
	return this->missingResource.c_str();
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};










