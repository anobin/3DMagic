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
/** Header file for GraphicalBody class
 *
 * @file GraphicalBody.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_GRAPHICAL_BODY_H
#define MAGIC3D_GRAPHICAL_BODY_H

#include "../Models/Model.h"
#include "../Math/Position.h"
#include "../Util/Color.h"
#include "../Graphics/Texture.h"
#include "../Exceptions/MagicException.h"
#include "../Shaders/Shader.h"

namespace Magic3D
{


/** Graphical body of an object. Contains graphical properties
 * (texture, position, model).
 */
class GraphicalBody
{
protected:
	/// model to use to represent this object
	Model& model;
	
	/// shader to use to render this body
	Shader* shader;
	
	/// base texture of object
	Texture* baseTexture;

	
public:
	/// standard constructor
	inline GraphicalBody(Model& model): model(model), shader(NULL), baseTexture(NULL) {}
	
	/// destructor
	virtual ~GraphicalBody();


	/** Draw the object
	 */
	inline void draw()
	{
		model.draw();
	}
	
	/// get the model used
	inline Model& getModel()
	{
		return model;
	}
	
	/// set the shader to render with
	inline void setShader( Shader& shader )
	{
	     // ensure that shader can render the model
	     // TODO
	     
	     // set the shader for the body
	     this->shader = &shader;
	}
	
	/// get the shader to render with
	inline Shader* getShader()
	{
	    return this->shader;    
	}
	
	/// set the texture
	inline void setBaseTexture(Texture& texture)
	{
		this->baseTexture = &texture;
	}
	
	/// get the texture
	inline Texture* getBaseTexture()
	{
		return this->baseTexture;
	}

};





};



#endif





















