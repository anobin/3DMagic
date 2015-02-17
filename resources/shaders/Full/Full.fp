#version 120

precision highp float;

// diffuse texture map
uniform sampler2D textureMap;

uniform sampler2D normalMap;
uniform float normalMapping = 0;

// specular color
uniform vec3 specularColor = vec3(0.7, 0.7, 0.7);
// specular power (sharpness of highlight)
uniform float specularPower = 128.0;

// ambient color and factor
//uniform vec3 ambientColor = vec3(0.2, 0.2, 0.2);
uniform float ambientFactor = 0.1;

// light position in world space
uniform vec3 lightPosition;
// light max distance
uniform float lightMaxDistance = 40.0;
// light decay mode
uniform int lightDecayFunc = 0;

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
    // light travels forever if atten distance is 0
    if (lightMaxDistance != 0.0)
    {
        float distance = distance(lightPosition.xyz, vPositionWorldSpace.xyz);
        float factor;
        
        if (lightDecayFunc == 1) // None
        {
            factor = 1.0;
            if (distance > lightMaxDistance)
                factor = 0.0;
        }
        else if (lightDecayFunc == 2) // Linear
        {
            factor = 1.0 - (distance / lightMaxDistance);
        }
        else //if (lightDecayFunc == 0) // Exponential
        {
            // exponential decay gets to less than 0.01 at -5
            factor = exp(-(distance / lightMaxDistance) * 5.0);
        }
        
        return max(factor, 0.0);
    }
    else
        return 1.0;
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
    
    
    float lightFactor = calculateLightAttenFactor();
    
    vec3 H = normalize(L + V);
    
    vec4 diffuseColor = texture2D(textureMap, vTexCoord);
    
    vec3 ambient = diffuseColor.rgb * ambientFactor * lightFactor;
    vec3 diffuse = max(dot(N,L), 0.0) * diffuseColor.rgb * lightFactor;
    vec3 specular = pow(max(dot(N,H), 0.0), specularPower) * specularColor * lightFactor; 
    
    gl_FragColor = vec4(ambient + diffuse + specular, diffuseColor.a);
}