
#include "ModelLoader.h"
#include "models\ModelLoader3DS.h"

namespace Magic3D
{
	
ModelLoaders& ModelLoaders::getSingleton()
{
	static ModelLoaders* loaders = nullptr;
	if (loaders == nullptr)
	{
		loaders = new ModelLoaders();
		loaders->registerLoader("3ds", std::make_shared<ModelLoader3DS>());
	}
	return *loaders;
}
	
	
};




