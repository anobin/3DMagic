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
/** Header file for StaticFont class
 *
 * @file StaticFont.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_STATIC_FONT_H
#define MAGIC3D_STATIC_FONT_H

#include "../Exceptions/MagicException.h"
#include "Character.h"
#include "magic_throw.h"

#include <map>

// everything is in 3DMagic namespace
namespace Magic3D
{
    
	
/** Represents a single text font that can be used by images to write text.
 * Static in the sense that all the characters contained are already rasterized
 * to a certain size bitmap and that only the characters explicitly added to
 * the font are contained within it. This font is not as easy to use as a 
 * dynamic font, but is much more efficient.
 */
class StaticFont
{
private:
	/// collection of characters
	std::map<unsigned int, Character*> chars;
	
	/// missing character
	Character missingChar;
	
public:	
	/// standard constructor for empty font
	inline StaticFont(const Character& missingChar): missingChar(missingChar) {}
	
	virtual ~StaticFont();
	
	inline int getCharCount()
	{
	    return chars.size();
	}
	
	inline bool isCharIncluded(unsigned int charcode)
	{
	    return (chars.find(charcode) != chars.end());
	}
	
	inline const Character& getChar(unsigned int charcode)
	{
	    std::map<unsigned int, Character*>::iterator it;
	    it = chars.find(charcode);
	    if (it == chars.end())
	        return missingChar;
	    return *it->second;
	}
	
	inline void setChar(const Character& character)
	{
	    std::map<unsigned int, Character*>::iterator it;
	    it = chars.find(character.getCharCode());
	    if (it != chars.end())
	        delete it->second; // free old char, if one
	    chars.insert( std::pair<unsigned int, Character*>(character.getCharCode(), 
	        new Character(character)) );
	}
	
	// this method assumes the parameter is allocated on the heap and takes
	// over management of the memory, avoids a memory copy
	inline void setChar(Character* character)
	{
	    std::map<unsigned int, Character*>::iterator it;
	    it = chars.find(character->getCharCode());
	    if (it != chars.end())
	        delete it->second; // free old char, if one
	    chars.insert( std::pair<unsigned int, Character*>(character->getCharCode(), 
	        character) );
	}
	
	
	
	
};
	
	
	
	
	
	
	
	
	
}


#endif











