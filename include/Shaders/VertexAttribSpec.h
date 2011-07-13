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
    
    /** The types of built-in attributes
     */
    enum BuiltInAttributeType
    {
        POSITION     = 0,   // float vector 4
        NORMAL       = 1,   // float vector 3
        COLOR        = 2,   // float vector 4
        BASE_TEXTURE = 3,   // float vector 2
        BUILTIN_ATTRIB_COUNT = 4
    };
    
    /** The type for a named attribute
     */
    struct NamedAttribType
    {
        int index;
        int components;
        VertexArray::DataTypes type;
        
        inline NamedAttribType( const NamedAttribType& d )
        {
            this->index = d.index;
            this->components = d.components;
            this->type = d.type;
        }
    };

private:
    /// GL indexes for built-in types, -1 for invalid
    int builtin_index[ (int) BUILTIN_ATTRIB_COUNT ];
    
    /// map of name to GL index for named attributes
    std::map<std::string, NamedAttribType*> named_index;
    
protected:
    
    /* Friend access is given to Shader class to aid in constructing a spec
     */
    friend class Magic3D::Shader;
    
    /** Set the index for a built-in attribute
     * @param type the type to set the index for
     * @param index the index to set
     */
    inline void setBuiltIn( BuiltInAttributeType type, int index )
    {
        builtin_index[ (int) type ] = index;
    }
     
    /** Set the index for a named attribute
     * @param name the name to set
     * @param data data to set for named attribute
     */
    inline void setNamed( const char* name, const NamedAttribType& data )
    {
        NamedAttribType* m = new NamedAttribType( data );
        named_index.insert( std::pair<std::string, NamedAttribType*> 
            ( std::string( name ), m ) );
    }
    
    /// default constructor
    inline VertexAttribSpec()
    {
        // ensure every built-in type has an invalid index
        for ( int i=0; i < BUILTIN_ATTRIB_COUNT; i++)
        {
            builtin_index[ i ] = -1;
        }
    }

public:
    
    /// deconstructor
    inline ~VertexAttribSpec()
    {
        // free all allocated named types
        std::map<std::string, NamedAttribType*>::iterator it = named_index.begin();
        for( ; it != named_index.end(); it++ )
        {
            delete it->second;
        }
    }
    
    /** Get the GL index for a built-in attribute
     * @param attribute the attribute to get the index for
     * @return the index for the attribute, -1 for not supported
     */
     inline int getBuiltInAttrib( BuiltInAttributeType attribute ) const
     {
         return builtin_index[ (int) attribute ];
     }
     
     /** Get the data for a named attribute
      * @param name the name of the attribute
      * @return the data for the attribute, or NULL for unsupported
      */
      inline const NamedAttribType* getNamedAttrib( const char* name ) const
      {
          std::string n( name );
          std::map<std::string, NamedAttribType*>::const_iterator it =
            named_index.find( n );
          if ( it == named_index.end() )
              return NULL;
          return it->second;
      }
          
    
    
};
    



};




#endif




















