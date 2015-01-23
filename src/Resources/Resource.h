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
/** Header file for Resource class
 * @file Resource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_RESOURCE_H
#define MAGIC3D_RESOURCE_H

#include <string>

namespace Magic3D
{
	
/** base class for all resources
 */
class Resource
{	
private:
	/// default constructor, can't be used
	inline Resource(): name(NULL) {}

protected:
	/// the name of this resource
	std::string name;
	
	/// standard constructor
	inline Resource(const std::string& name): name(name) {}

public:	
	/// destructor
	virtual ~Resource();
	
	/** get the name of this resource
	 * @return const reference to the name of thisa resource.
	 */
	inline const std::string& getName() const
	{
		return name;
	}

};



};






#endif









