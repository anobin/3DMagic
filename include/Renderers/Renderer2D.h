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
/** Header file for Renderer2D class
 *
 * @file Renderer2D.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_2D_RENDERER_H
#define MAGIC3D_2D_RENDERER_H

#include <vector>

// local includes
#include "../Objects/Object.h"
#include "../Graphics/GraphicalBody.h"
#include "../Math/Position.h"
#include "../Math/Point3.h"
#include "../Shaders/Shader.h"
#include "../Resources/ResourceManager.h"
#include "../Cameras/Camera.h"
#include "Renderer.h"


namespace Magic3D
{
	
/** Contains logic to render an object using a the 2DShader.
 */
class Renderer2D: public Renderer 
{
protected:
    Shader* shader;
    
    const Camera* camera;
    
public: 
    
    /// standard constructor
    Renderer2D(ResourceManager& resourceManager);
    
	/// destructor
	virtual ~Renderer2D();

	virtual void setup (unsigned int pass);

	virtual void render( Object* object, unsigned int pass );
	
	virtual unsigned int getPassCount();
	
	inline void setCamera( const Camera* camera )
	{
	    this->camera = camera;
	}
	
};

	
};



#endif











