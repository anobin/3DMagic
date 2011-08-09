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
/** Header file for Camera2D class
 *
 * @file Camera2D.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_CAMERA_2D_H
#define MAGIC3D_CAMERA_2D_H

#include "../Math/Vector3.h"
#include "../Math/Position.h"
#include "Camera.h"

#include <iostream>



namespace Magic3D
{
    
/** Represents a 2D camera, used when rendering GUI objects.
 */
 class Camera2D : public Camera
{
private:
    // doesn't really matter
    Position position;
    
public:
    inline Camera2D() {}
    
    virtual ~Camera2D();
    
    virtual const Position& getPosition() const;
    
};   
    

    
};














#endif









