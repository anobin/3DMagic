 
#ifndef MAGIC3D_LIGHT_H
#define MAGIC3D_LIGHT_H

#include <Math\Position.h>
#include <Util\Color.h>

namespace Magic3D
{


class Light
{
public:
    Vector3 location;
    bool locationLess;
    Vector3 direction;
    Scalar angle;

    Scalar intensity;
    Scalar attenuationFactor;

    Scalar ambientFactor;

    Color lightColor;

    Light() :
        location(0, 0, 0),
        locationLess(false),
        direction(0, 1, 0),
        angle(-1.0f),
        intensity(1.0f), 
        attenuationFactor(0.3f), 
        ambientFactor(0.01f),
        lightColor(Color::WHITE) 
    {}
};


};



#endif