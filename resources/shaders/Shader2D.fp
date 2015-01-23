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
/** Fragment shader for the Shader2D shader
 *
 * @file Shader2D.fp
 * @author Andrew Keating
 */
#version 120


// incoming tex coord from vertex shader
varying vec2 vTexCoord;

// texture sample map
uniform sampler2D textureMap;

void main(void)
{ 
	gl_FragColor = texture2D(textureMap, vTexCoord);
}