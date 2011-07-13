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

#include "Handle.h"
#include "Resource.h"
#include "TextResource.h"
#include "Images/SingleColor2DResource.h"
#include "../Exceptions/MagicException.h"
#include "../Exceptions/ResourceNotFoundException.h"
#include <string>
#include <map>


namespace Magic3D
{


/** Manages access to resources (text, image, raw data, models, etc.)
 */
class ResourceManager
{
private:
	/// mapping of resource names and resources
	std::map<std::string, Resource*> resources;
	
	/// directory where resources are contained
	std::string resourceDir;
	
	/// default constructor
	inline ResourceManager() {}
	
	/// increase handle count
	inline void acquire(const std::string& name)
	{
		std::map<std::string, Resource*>::iterator it = 
			resources.find(name);
		if (it == resources.end())
			throw MagicException("Tried to acquire non-existent resource");
		it->second->handleCount += 1;
	}
	
	/// decrease handle count
	inline void release(const std::string& name)
	{
		std::map<std::string, Resource*>::iterator it = 
			resources.find(name);
		if (it == resources.end())
			throw MagicException("Tried to release non-existent resource");
		it->second->handleCount -= 1;
		if (it->second->handleCount <= 0)
		{
			delete it->second;
			resources.erase(it);
		}
	}
	
public:
	friend class Resource;
	
	/** Standard constructor
	 * @param resourceDir the directory where resources are located
	 */
	inline ResourceManager(const char* resourceDir): resourceDir(resourceDir)
		{ /*intentionally left blank*/ }
		
	/// destructor
	virtual ~ResourceManager();
		
	/** Check if a resource exists, to be to avoid exceptions for optional resources
	 * @param name the name of the resource
	 * @return true for exists, false otherwise
	 */
	bool doesResourceExist(const char* name);
		
	/** get a resource
	 * @param name the name of the resource including any extra path info
	 * @return handle to text resource
	 */
	template <class T>
	inline Handle<T> get(const char* name)
	{
		// check if resource is already loaded
		std::map<std::string, Resource*>::iterator it = resources.find(name);
		if (it != resources.end())
		{
			// TODO: find a good way to determine if this cast is valid
			return Handle<T>(*((T*)(*it).second));
		}
		
		// otherwise, create new text resource
		if (!this->doesResourceExist(name))
			throw ResourceNotFoundException(name);
		T* r = new T((resourceDir + "/" + name).c_str(), std::string(name), *this);
		resources.insert(std::pair<std::string, Resource*>(name, r));
		Handle<T> h(*r);
		return h;
	}
	
	/** inject a SingleColor2D resource into the resource manager
	 * @param name the name to be given to the resource, the type name and
	 * a colon will be prepended to the name given (SingleColor2D:)
	 * @param color the color to make the image
	 * @return handle to new SingleColor2D resource
	 */
	inline Handle<SingleColor2DResource> injectSingleColor2D(const char* name,
			const Color& color)
	{
		// prefix typename to avoid filesystem name collisions
		std::string n;
		n = std::string("SingleColor2D:") + std::string(name);
		
		// create resource and inject into manager
		SingleColor2DResource* r = new SingleColor2DResource(color, n, *this);
		resources.insert(std::pair<std::string, Resource*>(n, r));
		return Handle<SingleColor2DResource>(*r);
	}
	
};




};






#endif








