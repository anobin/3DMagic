#version 120

// diffuse texture map
uniform sampler2D textureMap;

// specular color
uniform vec3 specularColor = vec3(0.7, 0.7, 0.7);
// specular power (sharpness of highlight)
uniform float specularPower = 128.0;

// ambient color and factor
//uniform vec3 ambientColor = vec3(0.2, 0.2, 0.2);
uniform float ambientFactor = 0.1;

// input from previous stage
varying vec3 vNormal;       // normal in view space
varying vec2 vTexCoord;     // texture coord
varying vec3 vViewDir;      // vector from vertex to camera (in view space)
varying vec3 vLightDir;     // vector from vertex to light (in view space) 
varying float vLightFactor; // factor of light intensity (based on attenuation)

void main(void)
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(vLightDir);
    vec3 V = normalize(vViewDir);
    
    vec3 H = normalize(L + V);
    
    vec4 diffuseColor = texture2D(textureMap, vTexCoord);
    
    vec3 ambient = diffuseColor.rgb * ambientFactor * vLightFactor;
    vec3 diffuse = max(dot(N,L), 0.0) * diffuseColor.rgb * vLightFactor;
    vec3 specular = pow(max(dot(N,H), 0.0), specularPower) * specularColor * vLightFactor; 
    
    gl_FragColor = vec4(ambient + diffuse + specular, diffuseColor.a);
}