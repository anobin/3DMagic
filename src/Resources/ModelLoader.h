#ifndef MAGIC3D_MODEL_LOADER_H
#define MAGIC3D_MODEL_LOADER_H

#include "./Resource.h"
#include "../Math/Matrix4.h"
#include "Objects\Model.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Magic3D
{

class ModelLoader
{
public:
	virtual std::shared_ptr<Model> getModel(const std::string& path) const = 0; 

};

class ModelLoaders
{
	std::map<std::string, std::shared_ptr<ModelLoader>> loaders;

public:
	static ModelLoaders& getSingleton();

	inline void registerLoader(const std::string& ext, std::shared_ptr<ModelLoader> loader)
	{
		loaders.insert(std::pair<std::string, std::shared_ptr<ModelLoader>>(ext, loader));
	}

	inline void unregisterLoader(const std::string& ext)
	{
		auto it = loaders.find(ext);
		if(it != loaders.end())
			loaders.erase(it);
	}

	inline std::shared_ptr<ModelLoader> get(const std::string ext)
	{
		auto it = loaders.find(ext);
		if (it == loaders.end())
			return nullptr; // TODO: add exception
		return it->second;
	}
};


};
















#endif













