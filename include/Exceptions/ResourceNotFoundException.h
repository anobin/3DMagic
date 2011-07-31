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
/** Header file for ResourceNotFoundException class
 *
 * @file ResourceNotFoundException.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_RESOURCE_NOT_FOUND_EXCEPTION_H
#define MAGIC3D_RESOURCE_NOT_FOUND_EXCEPTION_H

#include "MagicException.h"


namespace Magic3D
{
	
/** Thrown when a requested resource is not found
 */
class ResourceNotFoundException : public MagicException
{
protected:
	/// default constructor
	inline ResourceNotFoundException(): MagicException() {}
	
	/// the name of the resource that was not found
	std::string missingResource;
	
public:
	/** Standard constructor
	 * @param resource the resource that cannot be found
	 * @param file the file the error occurred in
	 * @param line the line of the file where the error occurred
	 */
	ResourceNotFoundException(const char* resource, const char* file, int line);

	/// destructor
	virtual ~ResourceNotFoundException() throw();
	
	/** Retrieve error message
	 * @return the error message
	 */
	virtual const char* what() const throw();
	
	/** Retrieve the resource that could not be found
	 * @return name of the resource that could not be found
	 */
	virtual const char* getMissingResourceName() const throw();

	
};

#define throw_ResourceNotFoundException(x) (throw Magic3D::ResourceNotFoundException((x), __FILE__, __LINE__))
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};

#endif












 
 
