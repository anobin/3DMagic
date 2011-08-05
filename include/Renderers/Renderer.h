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
/** Header file for Renderer class
 *
 * @file Renderer.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_RENDERER_H
#define MAGIC3D_RENDERER_H


// local includes
#include "../Graphics/GraphicalBody.h"


namespace Magic3D
{
	
/** Contains logic to render an object using a shader.
 */
class Renderer
{
protected:
    
    /// standard constructor
    inline Renderer() {}
    
public: 
	/// destructor
	virtual ~Renderer();

	virtual void render( const GraphicalBody& body ) = 0;

};

	
};



#endif











