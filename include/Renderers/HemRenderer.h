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
/** Header file for HemRenderer class
 *
 * @file HemRenderer.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_HEM_RENDERER_H
#define MAGIC3D_HEM_RENDERER_H

#include <vector>

// local includes
#include "../Objects/Object.h"
#include "../Graphics/GraphicalBody.h"
#include "../Math/Position.h"
#include "../Math/Point3.h"
#include "../Shaders/Shader.h"
#include "../Resources/ResourceManager.h"


namespace Magic3D
{
	
/** Contains logic to render an object using a the HemisphereTexShader.
 */
class HemRenderer
{
protected:
    const Position* camera;
    
    const Point3* lightSource;
    
    Shader* shader;
    
    const Color* skyColor;
    
    const Color* groundColor;
    
    const Matrix4* projectionMatrix;
    
public: 
    
    /// standard constructor
    HemRenderer(ResourceManager& resourceManager);
    
	/// destructor
	virtual ~HemRenderer();

	virtual void render( const std::vector<Object*>& bodies );

	inline void setCamera( const Position& camera )
	{
	    this->camera = &camera;
	}
	
	inline void setLightSource( const Point3& lightSource )
	{
	    this->lightSource = &lightSource;
	}
	
	inline void setSkyColor( const Color& skyColor )
	{
	    this->skyColor = &skyColor;
	}
	
	inline void setGroundColor( const Color& groundColor )
	{
	    this->groundColor = &groundColor;
	}
	
	inline void setProjectionMatrix( const Matrix4& projectionMatrix )
	{
	    this->projectionMatrix = &projectionMatrix;
	}
	
};

	
};



#endif











