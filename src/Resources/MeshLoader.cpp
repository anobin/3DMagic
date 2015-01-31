
#include "MeshLoader.h"
#include "models\MeshLoader3DS.h"

namespace Magic3D
{
	
MeshLoaders& MeshLoaders::getSingleton()
{
	static MeshLoaders* loaders = nullptr;
	if (loaders == nullptr)
	{
		loaders = new MeshLoaders();
		loaders->registerLoader("3ds", std::make_shared<MeshLoader3DS>());
	}
	return *loaders;
}
	
	
};




