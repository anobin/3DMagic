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

#include <iostream>



namespace Magic3D
{
    
/** Represents a First-Person camera. Contains math to move
 * and rotate a First-Person camera.
 */
class FPCamera
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
    inline void panView(float x, float y)
    {
        // rotate on Y-axis (around X-axis)
        // rotate with rederence to local coords
        position.rotateLocal(y / 180.0f * M_PI, Vector3(1.0f, 0.0f, 0.0f));
    
        // check Y-axis bounds
        Vector3& forward = position.getForwardVector();
        Vector3& up = position.getUpVector();
        // enforce bounds by never allowing the up (Y-axis) vector to go negative
        // correct one degree at time
        while (up.getY() < 0)
        {
            if (forward.getY() > 0)
                position.rotateLocal(1.0f / 180.0f * M_PI, Vector3(1.0f, 0.0f, 0.0f));
            else
                position.rotateLocal(-1.0f / 180.0f * M_PI, Vector3(1.0f, 0.0f, 0.0f));
        }
        
        // rotate on X-axis (around Y-axis)
        // rotate with reference to world coords, since we are standing on the 'ground'
        position.rotate(x * (M_PI / 180.0f), Vector3(0.0f, 1.0f, 0.0f));
        Matrix4 rotMat;
        rotMat.createRotationMatrix(x * (M_PI / 180.0f), 0, 1, 0);
        facing.transform(rotMat); 
    }
    
    
    
    inline const Position& getPosition()
    {
        return this->position;
    }
    
    inline void setLocation( const Point3& location)
    {
        position.getLocation().set(location);
    }
    
    inline void lookat( const Point3& point )
    {
        // Three conditions for first-person view:
        // 1) up.y is always positive, we don't stand on our head
        // 2) cross.y is always 0, we don't lean/roll
        // 3) facing.y is always 0, we only face in the xz plane
        
        // get references
        const Point3& location = position.getLocation();
        Vector3& forward = position.getForwardVector();
        Vector3& up = position.getUpVector();
        Vector3 cross;
        Vector3 wUp(0,1,0);
        
        // calc new forward from location and point to look at
        forward.setX(point.getX() - location.getX());
        forward.setY(point.getY() - location.getY());
        forward.setZ(point.getZ() - location.getZ());
        forward.normalize();
        
        // calc cross vector from forward and world up vectors
        // this ensures that cross.y is 0
        cross.crossProduct( wUp, forward ); // order?
        cross.normalize();
        
        // calc new local up vector, now that the other two vectors are good
        up.crossProduct( forward, cross ); // order?
        
        // use world up and our cross vector to calculate facing
        facing.crossProduct( cross, wUp );
        
    }
    
    
};   
    

    
};














#endif









