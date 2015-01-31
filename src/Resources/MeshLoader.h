#ifndef MAGIC3D_BATCH_LOADER_H
#define MAGIC3D_BATCH_LOADER_H

#include "./Resource.h"
#include <Graphics/Mesh.h>
#include "../Math/Matrix4.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Magic3D
{

class MeshLoader
{
public:
	virtual std::shared_ptr<Meshes> getMeshes(const std::string& path) const = 0; 

};

class MeshLoaders
{
	std::map<std::string, std::shared_ptr<MeshLoader>> loaders;

public:
	static MeshLoaders& getSingleton();

	inline void registerLoader(const std::string& ext, std::shared_ptr<MeshLoader> loader)
	{
		loaders.insert(std::pair<std::string, std::shared_ptr<MeshLoader>>(ext, loader));
	}

	inline void unregisterLoader(const std::string& ext)
	{
		auto it = loaders.find(ext);
		if(it != loaders.end())
			loaders.erase(it);
	}

	inline std::shared_ptr<MeshLoader> get(const std::string ext)
	{
		auto it = loaders.find(ext);
		if (it == loaders.end())
			return nullptr; // TODO: add exception
		return it->second;
	}
};


};
















#endif













