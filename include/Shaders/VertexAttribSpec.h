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
/** Header file for VertexAttribSpec class
 *
 * @file VertexAttribSpec.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VERTEX_ATTRIB_SPEC_H
#define MAGIC3D_VERTEX_ATTRIB_SPEC_H

#include <map>
#include <string>
#include <stdarg.h>
#include <Exceptions/MagicException.h>
#include <Graphics/VertexArray.h>

namespace Magic3D
{

/// forward declaration of shader class for friend declaration
class Shader;
    
  
/** Serves as the specification of vertex attributes a shader contains. Note that
 * a model built with one spec may not work with a different spec, even is they
 * seem compatable.
 *
 * This spec is generated for a shader based on the attributes that have been
 * set for it. The spec is then either passed to each model as it is being
 * constructed or set as the global spec. Each model, when constructed, will 
 * use the spec it is given or retrieve the global spec if none is given. The
 * spec is passed to the vertex handler used by the model. The vertex handler 
 * uses the spec to determine the attribute bindings that should be used for
 * each built-in or named atribute, as well as the type and number of components
 * for each attribute (as the the type and components for the built-in's are set).
 *
 * The built-in attributes are simply common attributes that have fixed types and
 * number of components. The named attributes are used when extra per-vertex data
 * is required that is not covered by the built-in attributes. Many shaders will
 * find that the built-in attributes are all they need, though a shader not need
 * to support any of the built-in attributes.
 *
 * Models will set the attributes they need to for the model. If a shader does not
 * a attribute needed by a model, then the model will most likely throw an exception 
 * on construction; thus it is a good idea for every shader to support the built-in
 * attributes if possible (especially when using the built-in models).
 */
class VertexAttribSpec
{
public:
    /** The type for a attribute
     */
    struct AttribType
    {
        int index;
        int components;
        VertexArray::DataTypes type;
        
        inline AttribType() {}
        
        inline AttribType( const AttribType& d )
        {
            this->index = d.index;
            this->components = d.components;
            this->type = d.type;
        }
    };

private:    
    /// map of name to GL index for named attributes
    std::map<std::string, AttribType*> index;
    
protected:
    
    /* Friend access is given to Shader class to aid in constructing a spec
     */
    friend class Magic3D::Shader;
     
    /** Set the index for a named attribute
     * @param name the name to set
     * @param data data to set for named attribute
     */
    inline void setAttrib( const char* name, const AttribType& data )
    {
        AttribType* m = new AttribType( data );
        index.insert( std::pair<std::string, AttribType*> 
            ( std::string( name ), m ) );
    }
    
    /// default constructor
    inline VertexAttribSpec()
    {
    }

public:
    
    /// deconstructor
    inline ~VertexAttribSpec()
    {
        // free all allocated attribute types
        std::map<std::string, AttribType*>::iterator it = index.begin();
        for( ; it != index.end(); it++ )
        {
            delete it->second;
        }
    }
     
     /** Get the data for a attribute
      * @param name the name of the attribute
      * @return the data for the attribute, or NULL for unsupported
      */
      inline const AttribType* getAttrib( const char* name ) const
      {
          std::string n( name );
          std::map<std::string, AttribType*>::const_iterator it =
            index.find( n );
          if ( it == index.end() )
              return NULL;
          return it->second;
      }
          
    
    
};
    



};




#endif




















