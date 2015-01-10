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
/** Header file for ResourceManager class
 * 
 * @file ResourceManager.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_RESOURCE_MANAGER_H
#define MAGIC3D_RESOURCE_MANAGER_H

#include "Resource.h"
#include "../Exceptions/MagicException.h"
#include "../Exceptions/ResourceNotFoundException.h"
#include <string>
#include <map>
#include <memory>


namespace Magic3D
{


/** Manages access to resources (text, image, raw data, models, etc.)
 */
class ResourceManager
{
private:
	/// mapping of resource names and resources
	std::map<std::string, std::weak_ptr<Resource>> resources;
	
	/// directory where resources are contained
	std::string resourceDir;
	
	/// default constructor
	inline ResourceManager() {}
	
public:
	friend class Resource;
	
	/** Standard constructor
	 * @param resourceDir the directory where resources are located
	 */
	inline ResourceManager(std::string resourceDir): resourceDir(resourceDir)
		{ /*intentionally left blank*/ }
		
	/// destructor
	virtual ~ResourceManager();
		
	/** Check if a resource exists, to be to avoid exceptions for optional resources
	 * @param name the name of the resource
	 * @return true for exists, false otherwise
	 */
	bool doesResourceExist(std::string name);
		
	/** get a resource
	 * @param name the name of the resource including any extra path info
	 * @return handle to text resource
	 */
	template <class T>
	inline std::shared_ptr<T> get(std::string name)
	{
		// check if resource is already loaded
		auto it = resources.find(name);
		if (it != resources.end() && !it->second.expired())
		{
			std::shared_ptr<Resource> ptr = std::shared_ptr<Resource>(it->second);
			return std::dynamic_pointer_cast<T>(ptr);
		}
		
		// otherwise, create new resource

		// make sure file exists
		if (!this->doesResourceExist(name))
			throw_ResourceNotFoundException(name);

		// load resource
		std::shared_ptr<T> r = std::make_shared<T>(resourceDir + "/" + name, name);
		resources.insert(std::pair<std::string, std::weak_ptr<Resource>>(name, std::weak_ptr<T>(r)));

		return r;
	}
	
};




};






#endif








