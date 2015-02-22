 
#ifndef MAGIC3D_LIGHT_H
#define MAGIC3D_LIGHT_H

#include <Math\Position.h>
#include <Util\Color.h>

namespace Magic3D
{


class Light
{
public:
    Position position;

    Scalar intensity;
    Scalar attenuationFactor;

    Scalar ambientFactor;

    Color lightColor;

    Light() : intensity(1.0f), attenuationFactor(0.3f), ambientFactor(0.01f),
        lightColor(Color::WHITE) {}
};


};



#endif