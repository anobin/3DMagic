#version 120

// texture sample map
uniform sampler2D textureMap;


uniform vec3 specularColor = vec3(0.7, 0.7, 0.7);
uniform vec3 ambientColor = vec3(0.2, 0.2, 0.2);
uniform float ambientFactor = 0.2;
uniform float specularPower = 128.0;

// input from previous stage
varying vec3 vNormal;
varying vec2 vTexCoord;
varying vec3 vViewDir;
varying vec3 vLightDir;

void main(void)
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(vLightDir);
    vec3 V = normalize(vViewDir);
    vec3 nL = -L;
    
    vec3 H = normalize(L + V);
    
    vec4 diffuseColor = texture2D(textureMap, vTexCoord);
    
    vec3 ambient = ambientColor * ambientFactor;
    vec3 diffuse = max(dot(N,L), 0.0) * diffuseColor.rgb;
    vec3 specular = pow(max(dot(N,H), 0.0), specularPower) * specularColor;
    
    gl_FragColor = vec4(ambient + diffuse + specular, diffuseColor.a);
}