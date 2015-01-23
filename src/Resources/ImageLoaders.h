/* 
Copyright (c) 2015 Andrew Keating

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
#ifndef MAGIC3D_IMAGE_LOADERS_H
#define MAGIC3D_IMAGE_LOADERS_H

#include <map>
#include <memory>
#include <Graphics\Image.h>

namespace Magic3D
{

class ImageLoader
{
protected:
	inline ImageLoader() {}
public:
	virtual std::shared_ptr<Image> getImage(const std::string& path) const = 0;
};

class ImageLoaders
{
	std::map<std::string, std::shared_ptr<ImageLoader>> loaders;

	inline ImageLoaders() {}
public:
	static ImageLoaders& getSingleton();

	const std::shared_ptr<ImageLoader> get(const std::string& ext)
	{
		auto it = this->loaders.find(ext);
		if (it == this->loaders.end())
			return nullptr;
		return it->second;
	}

	void registerLoader(const std::string& ext, std::shared_ptr<ImageLoader> loader)
	{
		loaders.insert(std::pair<std::string, std::shared_ptr<ImageLoader>>(ext, loader)); 
	}

	void unregisterLoader(const std::string& ext)
	{
		loaders.erase(loaders.find(ext));
	}

};

};

#endif