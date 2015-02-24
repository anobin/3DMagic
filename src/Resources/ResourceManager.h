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
#include "MeshLoader.h"
#include <vector>
#include <Graphics\Mesh.h>
#include <tinyxml2.h>
#include <Util/Color.h>
#include <Graphics\Material.h>
#include <Graphics\MaterialBuilder.h>
#include <CollisionShapes\CollisionShape.h>
#include <CollisionShapes\BoxCollisionShape.h>


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
inline std::shared_ptr<Meshes> ResourceManager::_get<Meshes>(const std::string& fullPath)
{
	std::string ext = fullPath.substr(fullPath.find_last_of(".")+1);
	return MeshLoaders::getSingleton().get(ext)->getMeshes(fullPath);
}

template<>
inline std::shared_ptr<Shader> ResourceManager::_get<Shader>(const std::string& fullPath)
{
	auto text = this->_get<TextResource>(fullPath);
	std::string ext = fullPath.substr(fullPath.find_last_of(".")+1);
	
	// TODO: add else case and exception
	Shader::Type type = Shader::Type::COMPUTE;
	if (ext == "vp")
		type = Shader::Type::VERTEX;
	else if (ext == "fp")
		type = Shader::Type::FRAGMENT;

	return std::make_shared<Shader>(text->getText(), type);
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

    bool removeGammaCorrection = true;
    auto gammaNode = textureNode->FirstChildElement("removeGammaCorrection");
    if (gammaNode != nullptr)
    {
        auto gammaVal = gammaNode->GetText();
        if (gammaVal != nullptr && std::string(gammaVal) == "false")
            removeGammaCorrection = false;
    }

    auto texture = std::make_shared<Texture>(*image, removeGammaCorrection);
	
	// TODO: parse wrap mode and other texture properties

	return texture;
}


class GpuProgramParser
{
	std::map<std::string, GpuProgram::AttributeType> attributeMap;
	std::map<std::string, GpuProgram::AutoUniformType> uniformMap;

	GpuProgramParser()
	{
		attributeMap.insert(std::make_pair("VERTEX", GpuProgram::AttributeType::VERTEX));
		attributeMap.insert(std::make_pair("NORMAL", GpuProgram::AttributeType::NORMAL));
		attributeMap.insert(std::make_pair("COLOR", GpuProgram::AttributeType::COLOR));
		attributeMap.insert(std::make_pair("COLOR2", GpuProgram::AttributeType::COLOR2));
		attributeMap.insert(std::make_pair("TEX_COORD_0", GpuProgram::AttributeType::TEX_COORD_0));
		attributeMap.insert(std::make_pair("TANGENT", GpuProgram::AttributeType::TANGENT));
		attributeMap.insert(std::make_pair("BINORMAL", GpuProgram::AttributeType::BINORMAL));

		uniformMap.insert(std::make_pair("MODEL_MATRIX", GpuProgram::AutoUniformType::MODEL_MATRIX));
		uniformMap.insert(std::make_pair("VIEW_MATRIX", GpuProgram::AutoUniformType::VIEW_MATRIX));
		uniformMap.insert(std::make_pair("PROJECTION_MATRIX", GpuProgram::AutoUniformType::PROJECTION_MATRIX));
        uniformMap.insert(std::make_pair("MODEL_VIEW_MATRIX", GpuProgram::AutoUniformType::MODEL_VIEW_MATRIX));
        uniformMap.insert(std::make_pair("VIEW_PROJECTION_MATRIX", GpuProgram::AutoUniformType::VIEW_PROJECTION_MATRIX));
        uniformMap.insert(std::make_pair("MODEL_PROJECTION_MATRIX", GpuProgram::AutoUniformType::MODEL_PROJECTION_MATRIX));
        uniformMap.insert(std::make_pair("MODEL_VIEW_PROJECTION_MATRIX", GpuProgram::AutoUniformType::MODEL_VIEW_PROJECTION_MATRIX));
        uniformMap.insert(std::make_pair("NORMAL_MATRIX", GpuProgram::AutoUniformType::NORMAL_MATRIX));
		uniformMap.insert(std::make_pair("TEXTURE0", GpuProgram::AutoUniformType::TEXTURE0));
        uniformMap.insert(std::make_pair("SHININESS", GpuProgram::AutoUniformType::SHININESS));
        uniformMap.insert(std::make_pair("SPECULAR_COLOR", GpuProgram::AutoUniformType::SPECULAR_COLOR));
		uniformMap.insert(std::make_pair("LIGHT_LOCATION", GpuProgram::AutoUniformType::LIGHT_LOCATION));
        uniformMap.insert(std::make_pair("LIGHT_DIRECTION", GpuProgram::AutoUniformType::LIGHT_DIRECTION));
        uniformMap.insert(std::make_pair("LIGHT_ANGLE", GpuProgram::AutoUniformType::LIGHT_ANGLE));
        uniformMap.insert(std::make_pair("LIGHT_INTENSITY", GpuProgram::AutoUniformType::LIGHT_INTENSITY));
        uniformMap.insert(std::make_pair("LIGHT_ATTENUATION_FACTOR", GpuProgram::AutoUniformType::LIGHT_ATTENUATION_FACTOR));
        uniformMap.insert(std::make_pair("LIGHT_AMBIENT_FACTOR", GpuProgram::AutoUniformType::LIGHT_AMBIENT_FACTOR));
        uniformMap.insert(std::make_pair("LIGHT_COLOR", GpuProgram::AutoUniformType::LIGHT_COLOR));
        uniformMap.insert(std::make_pair("SHADOW_MATRIX", GpuProgram::AutoUniformType::SHADOW_MATRIX));
        uniformMap.insert(std::make_pair("SHADOW_MAP", GpuProgram::AutoUniformType::SHADOW_MAP));
		uniformMap.insert(std::make_pair("FLAT_PROJECTION", GpuProgram::AutoUniformType::FLAT_PROJECTION));
        uniformMap.insert(std::make_pair("NORMAL_MAP", GpuProgram::AutoUniformType::NORMAL_MAP));
        uniformMap.insert(std::make_pair("SHADOW_MATRIX", GpuProgram::AutoUniformType::SHADOW_MATRIX));
        uniformMap.insert(std::make_pair("SHADOW_MAP", GpuProgram::AutoUniformType::SHADOW_MAP));

	}

public:
	inline static GpuProgramParser& getSingleton()
	{
		static GpuProgramParser* parser = nullptr;
		if (parser == nullptr)
			parser = new GpuProgramParser();
		return *parser;
	}

	inline GpuProgram::AttributeType parseAttributeType(const std::string& text)
	{
		// TODO: add possible exception
		return this->attributeMap.find(text)->second;
	}

	inline GpuProgram::AutoUniformType parseAutoUniformType(const std::string& text)
	{
		// TODO: add possible exception
		return this->uniformMap.find(text)->second;
	}
	
};

template<>
inline std::shared_ptr<GpuProgram> ResourceManager::_get<GpuProgram>(const std::string& fullPath)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(fullPath.c_str());
	// TODO: check doc load error and throw exception

	// TODO: check nodes for null and throw exception
	tinyxml2::XMLElement* programNode = doc.FirstChildElement("GpuProgram");

	auto vertexProgram = this->get<Shader>(programNode->FirstChildElement("vertexShader")->Attribute("ref"));
	auto fragmentProgram = this->get<Shader>(programNode->FirstChildElement("fragmentShader")->Attribute("ref"));

	auto program = std::make_shared<GpuProgram>(vertexProgram, fragmentProgram);

	auto parser = GpuProgramParser::getSingleton();

	auto attributeNode = programNode->FirstChildElement("attribute");
	while (attributeNode != nullptr)
	{
		auto name = attributeNode->FirstChildElement("name")->GetText();
		auto typeText = attributeNode->FirstChildElement("type")->GetText();
		auto type = parser.parseAttributeType(typeText);
		program->bindAttrib(name, type);

		attributeNode = attributeNode->NextSiblingElement("attribute");
	}

	auto uniformNode = programNode->FirstChildElement("uniform");
	while(uniformNode != nullptr)
	{
		auto name = uniformNode->FirstChildElement("name")->GetText();
		
		auto valueNode = uniformNode->FirstChildElement("value");
		auto valueRef = valueNode->Attribute("ref");
		if (valueRef != nullptr)
		{
			auto value = parser.parseAutoUniformType(valueRef);
			program->addAutoUniform(name, value);
		}
		else
		{
			auto componentNode = valueNode->FirstChildElement("c");
			std::vector<float> components;
			while (componentNode != nullptr)
			{
				components.push_back(std::stof(componentNode->GetText()));
				componentNode = componentNode->NextSiblingElement("c");
			}

			float* data = new float[components.size()];
			for(unsigned int i=0; i < components.size(); i++)
				data[i] = components[i];
			program->addNamedUniform(name, VertexArray::FLOAT, components.size(), data);
			delete[] data;
		}

		uniformNode = uniformNode->NextSiblingElement("uniform");
	}

	program->link();
	return program;
}


template<>
inline std::shared_ptr<Material> ResourceManager::_get<Material>(const std::string& fullPath)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(fullPath.c_str());
	// TODO: check doc load error and throw exception

	// TODO: check nodes for null and throw exception
	tinyxml2::XMLElement* programNode = doc.FirstChildElement("Material");

	auto gpuProgramRef = programNode->FirstChildElement("gpuProgram")->Attribute("ref");
	auto gpuProgram = this->get<GpuProgram>(gpuProgramRef);

	auto textureRef = programNode->FirstChildElement("texture")->Attribute("ref");
	auto texture = this->get<Texture>(textureRef);

    auto normalMapNode = programNode->FirstChildElement("normalMap");
    std::shared_ptr<Texture> normalMap = nullptr;
    if (normalMapNode != nullptr)
    {
        normalMap = this->get<Texture>(normalMapNode->Attribute("ref"));
    }

	auto material = std::make_shared<Material>();
	MaterialBuilder builder;
	builder.begin(material.get());
	builder.setGpuProgram(gpuProgram);
	builder.setTexture(texture);
    builder.setNormalMap(normalMap);
	builder.end();

	return material;
}


template<>
inline std::shared_ptr<CollisionShape> ResourceManager::_get<CollisionShape>(const std::string& fullPath)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(fullPath.c_str());
	// TODO: check doc load error and throw exception

	// TODO: check nodes for null and throw exception
	tinyxml2::XMLElement* shapeNode = doc.FirstChildElement("CollisionShape");
	auto type = std::string(shapeNode->Attribute("type"));

	if (type == "BOX")
	{
		auto width = shapeNode->FirstChildElement("width")->GetText();
		auto height = shapeNode->FirstChildElement("height")->GetText();
		auto depth = shapeNode->FirstChildElement("depth")->GetText();

		return std::make_shared<BoxCollisionShape>(std::stof(width), std::stof(height), std::stof(depth));
	}

	return nullptr;
}


};






#endif








