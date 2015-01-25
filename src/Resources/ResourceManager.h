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
#include "TextResource.h"
#include "../Exceptions/MagicException.h"
#include "../Exceptions/ResourceNotFoundException.h"
#include "ImageLoaders.h"
#include "FontResource.h"
#include "fonts\TTFontResource.h"
#include <string>
#include <map>
#include <memory>
#include <fstream>
#include "BatchLoader.h"
#include <vector>
#include <Graphics\Mesh.h>
#include <tinyxml2.h>
#include <Util/Color.h>


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
	std::vector<std::string> resourceDirs;

	template<class T>
	inline std::shared_ptr<T> _get(const std::string& fullPath)
	{
		/* intentionally left blank, always need a specialization */
	}

	inline std::string getFullPath(const std::string& path)
	{
		for (const std::string& basePath : resourceDirs)
		{
			std::ifstream test;
			std::string fullPath = basePath + "/" + path;
			test.open(fullPath.c_str());
			if (test.is_open() && test.good())
				return fullPath;
		}
		return "";
	}
	
public:

	inline ResourceManager() {}
		
	/// destructor
	virtual ~ResourceManager();

	inline void addResourceDir(const std::string& dir)
	{
		this->resourceDirs.push_back(dir);
	}
		
	/** Check if a resource exists, to be to avoid exceptions for optional resources
	 * @param name the name of the resource
	 * @return true for exists, false otherwise
	 */
	inline bool doesResourceExist(const std::string& path)
	{
		return (this->getFullPath(path) != "");
	}
		
	/** get a resource
	 * @param name the name of the resource including any extra path info
	 * @return handle to resource
	 */
	template <class T>
	inline std::shared_ptr<T> get(const std::string& path)
	{
		// check if resource is already loaded
		auto it = resources.find(path);
		if (it != resources.end() && !it->second.expired())
		{
			std::shared_ptr<Resource> ptr = std::shared_ptr<Resource>(it->second);
			return std::dynamic_pointer_cast<T>(ptr);
		}
		
		// otherwise, create new resource

		// make sure file exists
		std::string fullPath = this->getFullPath(path);
		if (fullPath == "")
			throw_ResourceNotFoundException(path);

		// load resource
		std::shared_ptr<T> resource = this->_get<T>(fullPath);
		resources.insert(std::pair<std::string, std::weak_ptr<Resource>>(
			path, std::weak_ptr<Resource>(std::dynamic_pointer_cast<Resource>(resource))));
		return resource;
	}
	
};


template<>
inline std::shared_ptr<TextResource> ResourceManager::_get<TextResource>(const std::string& fullPath)
{
	std::ifstream file;
	file.open (fullPath, std::ios::binary );
	
	if (!file.is_open() || !file.good())
	{
		file.close();
		throw_MagicException("Failed to open text resource");
	}

	// get length of file:
	file.seekg (0, std::ios::end);
	int length = (int)file.tellg();
	file.seekg (0, std::ios::beg);

	// read all data into memory
	char* text = new char [length+1];
	file.read (text,length);
	text[length] = 0; // null terminated string
  
	return std::make_shared<TextResource>(text);
}

template<>
inline std::shared_ptr<Image> ResourceManager::_get<Image>(const std::string& fullPath)
{
	std::string ext = fullPath.substr(fullPath.find_last_of(".")+1);
	auto loader = ImageLoaders::getSingleton().get(ext);
	// TODO: add exception
	return loader->getImage(fullPath);
}

template<>
inline std::shared_ptr<FontResource> ResourceManager::_get<FontResource>(const std::string& fullPath)
{
	// TODO: add loaders for other formats
	return std::make_shared<TTFontResource>(fullPath, fullPath);
}

template<>
inline std::shared_ptr<Batches> ResourceManager::_get<Batches>(const std::string& fullPath)
{
	std::string ext = fullPath.substr(fullPath.find_last_of(".")+1);
	return BatchLoaders::getSingleton().get(ext)->getBatches(fullPath);
}

template<>
inline std::shared_ptr<Meshes> ResourceManager::_get<Meshes>(const std::string& fullPath)
{
	std::string ext = fullPath.substr(fullPath.find_last_of(".")+1);
	return std::make_shared<Meshes>(*BatchLoaders::getSingleton().get(ext)->getBatches(fullPath));
}

class ColorParser
{
	std::map<std::string, Color> namedColors;

	ColorParser()
	{
		namedColors.insert(std::make_pair("BLACK", Color::BLACK));
		namedColors.insert(std::make_pair("WHITE", Color::WHITE));
		namedColors.insert(std::make_pair("GREEN", Color::GREEN));
		namedColors.insert(std::make_pair("BLUE", Color::BLUE));
		namedColors.insert(std::make_pair("RED", Color::RED));
		namedColors.insert(std::make_pair("LIGHT_GREEN", Color::LIGHT_GREEN));
		namedColors.insert(std::make_pair("ORANGE", Color::ORANGE));
		namedColors.insert(std::make_pair("YELLOW", Color::YELLOW));
		namedColors.insert(std::make_pair("PURPLE", Color::PURPLE));
		namedColors.insert(std::make_pair("GRAY", Color::GRAY));
		namedColors.insert(std::make_pair("PINK", Color::PINK));
		namedColors.insert(std::make_pair("LIGHT_BLUE", Color::LIGHT_BLUE));
	}

public:

	static ColorParser& getSingleton()
	{
		static ColorParser* parser = nullptr;
		if (parser == nullptr)
			parser = new ColorParser();
		return *parser;
	}

	inline Color parse(tinyxml2::XMLElement* colorNode)
	{
		const char* name = colorNode->Attribute("name");
		if (name != nullptr)
		{
			// TODO: add exception for missing name
			return namedColors.find(name)->second;
		}

		// TODO: add individual component parsing
		return Color::PINK;
	}

};

template<>
inline std::shared_ptr<Texture> ResourceManager::_get<Texture>(const std::string& fullPath)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(fullPath.c_str());
	// TODO: check doc load error and throw exception

	// TODO: check nodes for null and throw exception
	tinyxml2::XMLElement* textureNode = doc.FirstChildElement("Texture");

	tinyxml2::XMLElement* imageNode = textureNode->FirstChildElement("image");
	const char* imageRef = imageNode->Attribute("ref");

	std::shared_ptr<Image> image;
	if (this->doesResourceExist(imageRef))
		image = this->get<Image>(imageRef);
	else
	{
		tinyxml2::XMLElement* fallbackNode = textureNode->FirstChildElement("fallback");
		tinyxml2::XMLElement* colorNode = fallbackNode->FirstChildElement("Color");
		Color color = ColorParser::getSingleton().parse(colorNode);
		image = std::make_shared<Image>(1, 1, color.getChannelCount(), color);
	}

	auto texture = std::make_shared<Texture>(*image);
	
	// TODO: parse wrap mode and other texture properties

	return texture;
}


};






#endif








