#version 120

precision highp float;

// diffuse texture map
uniform sampler2D textureMap;

uniform sampler2D normalMap;
uniform float normalMapping = 0;

// specular color
uniform vec3 specularColor;
// specular power (sharpness of highlight), shininess
uniform float specularPower = 128.0;

// ambient factor
uniform float ambientFactor;

// light position in world space
uniform vec3 lightPosition;
uniform float lightAttenuationFactor;
uniform float lightIntensity;

uniform vec3 gammaCorrectionFactor = vec3(1.0/2.2);

// input from previous stage
varying vec3 vNormal;       // normal in view space
varying vec2 vTexCoord;     // texture coord
varying vec3 vViewDir;      // vector from vertex to camera (in view space)
varying vec3 vViewDirN;      // vector from vertex to camera for normal map (in view space)
varying vec3 vLightDir;     // vector from vertex to light (in view space)
varying vec3 vLightDirN;     // vector from vertex to light for normal map (in view space)
varying vec4 vPositionWorldSpace; // position vector in world space

float calculateLightAttenFactor()
{
    float distance = distance(lightPosition.xyz, vPositionWorldSpace.xyz);
    return 1.0 / (1.0 + lightAttenuationFactor * pow(distance,2));
}

void main(void)
{
    vec3 N, L, V;
    
    if (normalMapping != 0)
    {
        N = normalize(texture2D(normalMap, vTexCoord).rgb * 2.0 - vec3(1.0));
        L = normalize(vLightDirN);
        V = normalize(vViewDirN); 
    }
    else
    {
        N = normalize(vNormal);
        L = normalize(vLightDir);
        V = normalize(vViewDir); 
    }
    
    
    float lightFactor = calculateLightAttenFactor() * lightIntensity;
    
    vec3 H = normalize(L + V);
    
    vec4 diffuseColor = texture2D(textureMap, vTexCoord);
    
    vec3 ambient = diffuseColor.rgb * ambientFactor * lightFactor;
    vec3 diffuse = max(dot(N,L), 0.0) * diffuseColor.rgb * lightFactor;
    vec3 specular = pow(max(dot(N,H), 0.0), specularPower) * specularColor * lightFactor; 
    
    vec3 color = ambient + diffuse + specular;
    gl_FragColor = vec4(pow(color,gammaCorrectionFactor), diffuseColor.a);
}