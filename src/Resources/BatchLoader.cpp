
#include "BatchLoader.h"
#include "models\BatchLoader3DS.h"

namespace Magic3D
{
	
BatchLoaders& BatchLoaders::getSingleton()
{
	static BatchLoaders* loaders = nullptr;
	if (loaders == nullptr)
	{
		loaders = new BatchLoaders();
		loaders->registerLoader("3ds", std::make_shared<BatchLoader3DS>());
	}
	return *loaders;
}
	
	
};




