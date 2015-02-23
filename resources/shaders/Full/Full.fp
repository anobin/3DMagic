#version 120

precision highp float;

uniform mat4   mvpMatrix;   // transforms from model space to clip space
uniform mat4   mvMatrix;    // transforms from model space to view space
uniform mat4   vMatrix;     // transforms from world space to view space
uniform mat4   mMatrix;     // transforms from model space to world space

// diffuse texture map
uniform sampler2D textureMap;

uniform sampler2D normalMap;
uniform float normalMapping = 0;

// specular color
uniform vec3 specularColor;
// specular power (sharpness of highlight), shininess
uniform float specularPower;

// ambient factor
uniform float ambientFactor;

// light position in world space
uniform vec4 lightPosition;
uniform float lightAttenuationFactor;
uniform float lightIntensity;
uniform vec3 lightColor;

uniform vec3 gammaCorrectionFactor = vec3(1.0/2.2);

// input from previous stage
varying vec3 vViewDir;      // vector from vertex to camera (in view space)

varying vec4 fragPosition;      // position of fragment in model space
varying vec3 fragNormal;        // normal vector in model space
varying vec3 fragTangent;       // tangent vector in model space
varying vec2 fragTexCoord;      // texture coordinate

float calculateLightAttenFactor()
{
    vec3 worldPos = (mMatrix * fragPosition).xyz;
    float distance = distance(lightPosition.xyz, worldPos.xyz);
    return 1.0 / (1.0 + lightAttenuationFactor * pow(distance,2));
}

void main(void)
{
    vec3 N = normalize(mat3(mvMatrix) * fragNormal);
    vec3 L = normalize(
        (vMatrix * vec4(lightPosition.xyz, 1.0)).xyz - (mvMatrix * fragPosition).xyz
    );
    vec3 V = normalize(-(mvMatrix * fragPosition).xyz); 
    
    // recalculate vectors for normal mapping (if enabled)
    if (normalMapping != 0)
    {
        vec3 T = normalize(mat3(mvMatrix) * fragTangent);
        vec3 B = cross(N, T);
    
        L = normalize(vec3(dot(L,T), dot(L,B), dot(L,N)));
        V = normalize(vec3(dot(V,T), dot(V,B), dot(V,N)));
        
        // calculate real normal from normal map
        N = normalize(texture2D(normalMap, fragTexCoord).rgb * 2.0 - vec3(1.0));
    }
    
    
    float lightFactor = calculateLightAttenFactor() * lightIntensity;
    
    vec3 H = normalize(L + V);
    
    vec4 diffuseColor = texture2D(textureMap, fragTexCoord);
    
    vec3 ambient = diffuseColor.rgb * lightColor.rgb * ambientFactor * lightFactor;
    vec3 diffuse = max(dot(N,L), 0.0) * diffuseColor.rgb * lightColor.rgb * lightFactor;
    vec3 specular = pow(max(dot(N,H), 0.0), specularPower) * specularColor * lightColor.rgb * lightFactor; 
    
    vec3 color = ambient + diffuse + specular;
    gl_FragColor = vec4(pow(color,gammaCorrectionFactor), diffuseColor.a);
}