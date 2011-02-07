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
/** Header file for Handle class
 *
 * @file Handle.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_HANDLE_H
#define MAGIC3D_HANDLE_H

#include "Resource.h"


namespace Magic3D
{


/** Handle to a single resource, used to track
 * resource use and deallocation.
 */
template <class T>
class Handle
{
private:
	/// reference to resource
	T* resource;
	
	/// default constructor
	inline Handle(): resource(NULL) {}

	
public:
	/** Standard constructor
	 * @param resource the resource to keep a handle to
	 */
	inline Handle(T& resource): resource(&resource)
	{
		resource.acquire();
	}
	
	/// copy constructor
	inline Handle(const Handle<T>& copy)
	{
		this->resource = copy.resource;
		resource->acquire();
	}
	
	/// destructor
	inline ~Handle()
	{
		resource->release();
	}
	
	
	/** Get resource
	 * @return the resource managed by this handle
	 */
	inline const T* get()
	{
		return resource;
	}

	/** get resource through operator
	 * @return the resource managed by this handle
	 */
	inline const T* operator()() // <- yeah it looks weird
	{
		return resource;
	}

};
	
	


};





#endif










