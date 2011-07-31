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
/** Implementation file for MagicAssertException class
 *
 * @file MagicAssertException.cpp
 * @author Andrew Keating
 */

#include <Exceptions/MagicAssertException.h>

#include <sstream>

namespace Magic3D
{

/** Standard constructor
 * @param comparison the comparison performed for the exception
 * @param file the file the error occurred in
 * @param line the line of the file where the error occurred
 */
MagicAssertException::MagicAssertException(const char* comparison, const char* file, int line): 
	comparison(comparison)
{
	std::ostringstream s;
	s << "(" << file << ":" << line << ") " << "Assertion: ( " << comparison << " )";
	this->MagicException::error = s.str();
}
	
/// destructor
MagicAssertException::~MagicAssertException() throw()
{
	/* intentionally left blank */
}
	
/** Retrieve error message
 * @return the error message
 */
const char* MagicAssertException::what() const throw()
{
	return this->MagicException::error.c_str();
}
	
	
	
/** Retrieve the comparison that was performed for the assert
 * @return string version of comparison performed
 */
const char* MagicAssertException::getComparison() const throw()
{
	return this->comparison.c_str();
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};










