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
/** Implementation file for ResourceManager class
 *
 * @file ResourceManager.cpp
 * @author Andrew Keating
 */

#include <Resources/ResourceManager.h>
#include <fstream>
#include <Exceptions/ResourceNotFoundException.h>

namespace Magic3D
{
	
	
/// destructor
ResourceManager::~ResourceManager()
{
	// delete all resources
	std::map<std::string, Resource*>::iterator it = this->resources.begin();
	for (; it != resources.end(); it++)
		delete (*it).second;
}
	
/** Check if a resource exists, to be to avoid exceptions for optional resources
 * @param name the name of the resource
 * @return true for exists, false otherwise
 */
bool ResourceManager::doesResourceExist(const char* name)
{
	std::ifstream test;
	test.open((resourceDir + "/" + name).c_str());
	if (test.is_open())
		return true;
	return false;
}


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};






