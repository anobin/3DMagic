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
/** Header file for MagicException class
 *
 * @file MagicException.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_EXCEPTION_H
#define MAGIC3D_EXCEPTION_H

#include <exception>
#include <string>


namespace Magic3D
{
	
/** Base class for all 3DMagic exceptions
 */
class _MagicException : public std::exception
{
protected:
	/// the error message
	std::string error;
	
	/// default constructor
	inline _MagicException(): error("None") {}
	
public:
	/** Standard constructor
	 * @param error the error string to use
	 * @param file the file the error occurred in
	 * @param line the line of the file where the error occurred
	 */
	_MagicException(const char* error, const char* file, int line);

	/// destructor
	virtual ~_MagicException() throw();
	
	/** Retrieve error message
	 * @return the error message
	 */
	virtual const char* what() const throw();

	
};

#define MagicException(x) (_MagicException((x), __FILE__, __LINE__))
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};

#endif












