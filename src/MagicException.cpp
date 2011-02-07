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
/** Implementation file for MagicException class
 *
 * @file MagicException.cpp
 * @author Andrew Keating
 */

// include header
#include <MagicException.h>

#include <sstream>

namespace Magic3D
{
	
	
/** Standard constructor
 * @param error the error string to use
 * @param file the file the error occurred in
 * @param line the line of the file where the error occurred
 */
MagicException::MagicException(const char* error, const char* file, int line)
{
	std::ostringstream s;
	s << "(" << file << ":" << line << ") " << error;
	this->error = s.str();
}


/// destructor
MagicException::~MagicException() throw()
{
		/* intentionally left blank */
}
	
/** Retrieve error message
 * @return the error message
 */
const char* MagicException::what() const throw()
{
	return this->error.c_str();
}
	
	
	
	
	
	
};	
	
	
	
	