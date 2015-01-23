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
/** Fragment shader for the SimpleADSShader shader
 *
 * @file SimpleADSShader.fp
 * @author Andrew Keating
 */

#version 120

uniform vec4    ambientColor;
uniform vec4    diffuseColor;   
uniform vec4    specularColor;

varying vec3 vVaryingNormal;
varying vec3 vVaryingLightDir;


void main(void)
{ 
    // Dot product gives us diffuse intensity
    float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));

    // Multiply intensity by diffuse color, force alpha to 1.0
    gl_FragColor.rgb = diff * diffuseColor.xyz;
	gl_FragColor.a = 1.0;

    // Add in ambient light
    gl_FragColor += ambientColor;


    // Specular Light, only process if there is also some diffuse intensity
    if(diff != 0) 
	{
		// find reflection vector
		vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir), normalize(vVaryingNormal)));
		// find specular intensity
		float specIntensity = max(0.0, dot(normalize(vVaryingNormal), vReflection));
		// apply shininess, default to 128
		float fSpec = pow(specIntensity, 128.0);
		// add in specular component
        gl_FragColor.rgb += fSpec * specularColor.xyz;
    }
}
    