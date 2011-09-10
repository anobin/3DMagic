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
/** Header file for FontResource class
 *
 * @file FontResource.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_FONT_RESOURCE_H
#define MAGIC3D_FONT_RESOURCE_H

#include "Resource.h"
#include "../Util/Character.h"

namespace Magic3D
{

/** Base class for all text font resources
 */
class FontResource : public Resource
{
protected:
	
	/// standard constructor
	inline FontResource(const std::string& name, ResourceManager& manager):
		Resource(name, manager) {}
		
public:
	/// destructor
	virtual ~FontResource();
	
	
	virtual void getChar(Character* c, unsigned int charcode, int width, int height) const = 0;
	
	virtual bool hasChar(unsigned int charcode) const = 0;
	
	virtual void getMissingChar(Character* c, int width, int height) const  = 0;

};


};



#endif







