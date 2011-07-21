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
/** Header file for MagicAssertException class
 *
 * @file MagicAssetException.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MAGIC_ASSERT_EXCEPTION_H
#define MAGIC3D_MAGIC_ASSERT_EXCEPTION_H

#include "MagicException.h"


namespace Magic3D
{
	
/** Thrown when a an assert happens if asserts are configured to
 * throw exceptions (MAGIC3D_ASSERT_DO_EXCEPTION)
 */
class _MagicAssertException : public _MagicException
{
protected:
	/// default constructor
	inline _MagicAssertException(): _MagicException() {}
	
	/// the string comparison that was performed for the assertion
	std::string comparison;
	
public:
	/** Standard constructor
	 * @param comparison the comparison performed for the assertion
	 * @param file the file the error occurred in
	 * @param line the line of the file where the error occurred
	 */
	_MagicAssertException(const char* comparison, const char* file, int line);

	/// destructor
	virtual ~_MagicAssertException() throw();
	
	/** Retrieve error message
	 * @return the error message
	 */
	virtual const char* what() const throw();
	
	/** Retrieve the comparison that was performed for the assert
	 * @return string version of comparison performed
	 */
	virtual const char* getComparison() const throw();

	
};

#define MagicAssertException(x) (_MagicAssertException( #x , __FILE__, __LINE__))
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};

#endif












 
 
