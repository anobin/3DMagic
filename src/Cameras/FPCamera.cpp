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
/** Implementation file for FPCamera class
 *
 * @file FPCamera.cpp
 * @author Andrew Keating
 */
 
#include <Cameras/FPCamera.h>

namespace Magic3D
{
    
FPCamera::~FPCamera()
{
    /* intentionally left blank */
}
    
const Position& FPCamera::getPosition() const
{
    return this->position;
}
    
/** Pan the view around in first-person mode
 * @param x the x degrees to pan
 * @param y the y degrees to pan
 */
 void FPCamera::panView(float x, float y)
{
    // rotate on Y-axis (around X-axis)
    // rotate with reference to local coords
    position.rotateLocal(y / 180.0f * ((float)M_PI), Vector3(1.0f, 0.0f, 0.0f));

    // check Y-axis bounds
    Vector3& forward = position.getForwardVector();
    Vector3& up = position.getUpVector();
    // enforce bounds by never allowing the up (Y-axis) vector to go negative
    // correct one degree at time
    while (up.y() < 0)
    {
        if (forward.y() > 0)
            position.rotateLocal(1.0f / 180.0f * ((float)M_PI), Vector3(1.0f, 0.0f, 0.0f));
        else
            position.rotateLocal(-1.0f / 180.0f * ((float)M_PI), Vector3(1.0f, 0.0f, 0.0f));
    }
    
    // rotate on X-axis (around Y-axis)
    // rotate with reference to world coords, since we are standing on the 'ground'
    position.rotate(x * (((float)M_PI) / 180.0f), Vector3(0.0f, 1.0f, 0.0f));
    Matrix4 rotMat;
    rotMat.createRotationMatrix(x * (((float)M_PI) / 180.0f), 0, 1, 0);
    facing = facing.transform(rotMat); 
}
    

void FPCamera::lookat( const Point3& point )
{
    // Three conditions for first-person view:
    // 1) up.y is always positive, we don't stand on our head
    // 2) cross.y is always 0, we don't lean/roll
    // 3) facing.y is always 0, we only face in the xz plane
    
    // get references
    const Point3& location = position.getLocation();
    Vector3 wUp(0,1,0);
    
    // calc new forward from location and point to look at
    Vector3 forward(
		point.getX() - location.getX(),
		point.getY() - location.getY(),
		point.getZ() - location.getZ()
		);
	forward = forward.normalize();
    
    // calc cross vector from forward and world up vectors
    // this ensures that cross.y is 0
	Vector3 cross = wUp.crossProduct(forward).normalize();
    
    // calc new local up vector, now that the other two vectors are good
    Vector3 up = forward.crossProduct(cross);
    
    // use world up and our cross vector to calculate facing
    this->facing = cross.crossProduct(wUp);

	this->position = Position(location, forward, up);
}
    
    
    
    
    
    
    
};









