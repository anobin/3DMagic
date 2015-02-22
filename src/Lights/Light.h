 
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
    Scalar diffuseFactor;
    Scalar specularFactor;

    Color lightColor;
    Color specularColor;

    Light() : intensity(1.0f), attenuationFactor(0.3f), ambientFactor(0.01f),
        diffuseFactor(1.0f), specularFactor(1.0f), lightColor(Color::WHITE),
        specularColor(0.7f, 0.7f, 0.7f) {}
};


};



#endif