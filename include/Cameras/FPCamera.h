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
/** Header file for FPCamera class
 *
 * @file FPCamera.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_FP_CAMERA_H
#define MAGIC3D_FP_CAMERA_H

#include "../Math/Vector3.h"
#include "../Math/Position.h"
#include "Camera.h"

#include <iostream>



namespace Magic3D
{
    
/** Represents a First-Person camera. Contains math to move
 * and rotate a First-Person camera.
 */
 class FPCamera : public Camera
{
private:
    Position position;
    
    Vector3 facing;
    
    float stepSpeed;
    
    float strafeSpeed;
    
public:
    inline FPCamera(): stepSpeed(1.0f), strafeSpeed(0.5f) 
    {
        // perfectly standing stance from beginning
        position.getForwardVector().set(0, 0, -1);
        position.getUpVector().set(0, 1, 0);
        facing.set(position.getForwardVector());
    }
    
    virtual ~FPCamera();
    
    inline void setStepSpeed( float s )
    {
        this->stepSpeed = s;
    }
    
    inline void setStrafeSpeed( float s )
    {
        this->strafeSpeed = s;
    }
    
    inline void step(float steps)
    {
        // we only step forward or back in the xz plane
        position.translate(facing.getX()*(stepSpeed*steps), 0.0f, 
            facing.getZ()*(stepSpeed*steps));
    }
    
    inline void strafe(float steps)
    {
        // we only strafe left or right in the xz plane
        Vector3 side;
        position.getLocalXAxis(side);
		position.translate(side.getX()*(strafeSpeed*steps), 0.0f, 
		    side.getZ()*(strafeSpeed*steps));
    }
    
    /** Pan the view around in first-person mode
     * @param x the x degrees to pan
     * @param y the y degrees to pan
     */
    void panView(float x, float y);
    
    virtual const Position& getPosition() const;
    
    inline void setLocation( const Point3& location)
    {
        position.getLocation().set(location);
    }
    
    void lookat( const Point3& point );
    
    inline void elevate( float a )
    {
        position.getLocation().setY( position.getLocation().getY()+a );
    }
    
};   
    

    
};














#endif









