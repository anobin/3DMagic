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
/** Header file for Camera class
 *
 * @file Camera.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_CAMERA_H
#define MAGIC3D_CAMERA_H

#include "../Math/Matrix4.h"
#include "../Math/Position.h"

#include <memory>

#include "ViewFrustum.h"

namespace Magic3D
{

/** Base class for all cameras
 */
class Camera
{
protected:
    /// default constructor
    inline Camera() {}

    /// projection matrix for camera
    Matrix4 projectionMatrix;

    ViewFrustum viewFrustum;
    
public:
    /// destructor
    virtual ~Camera();
    
    inline void setPerspectiveProjection(Scalar fov, Scalar aspectRatio, Scalar zMin,
        Scalar zMax )
    {
        projectionMatrix.createPerspectiveMatrix(fov, aspectRatio, zMin, zMax);
        viewFrustum.setCamProperties(fov, aspectRatio, zMin, zMax);
    }

    inline ViewFrustum& getViewFrustum()
    {
        viewFrustum.setPosition(this->getPosition());
        return this->viewFrustum;
    }
    
    inline void setOrthographicProjection(Scalar xMin, Scalar xMax, Scalar yMin, 
        Scalar yMax, Scalar zMin, Scalar zMax)
    {
        projectionMatrix.createOrthographicMatrix(xMin,xMax, yMin,yMax, zMin,zMax);
    }
    
    /** Get the projection matrix for this camera. The matrix is used when
     * rendering to create the view for a display.
     * @return the projection matrix for this camera
     */
    inline const Matrix4& getProjectionMatrix() const 
    {
        return this->projectionMatrix;
    }

    virtual const Position& getPosition() const = 0;


};

};

#endif





