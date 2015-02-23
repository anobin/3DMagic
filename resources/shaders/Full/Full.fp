#version 420 core

precision highp float;

uniform struct Transforms
{
    mat4   mvMatrix;    // transforms from model space to view space
    mat4   vMatrix;     // transforms from world space to view space
    mat4   mMatrix;     // transforms from model space to world space
} transforms;

uniform sampler2D textureMap;
uniform sampler2D normalMap;
uniform float normalMapping;
uniform struct Material 
{
    vec3 specularColor;
    float specularPower;
} material;

uniform struct Light
{
    vec4 position;
    float attenuationFactor;
    float intensity;
    vec3 color;
    float ambientFactor;
    vec3 direction;
    float angle;
} light;

uniform vec3 gammaCorrectionFactor = vec3(1.0/2.2);

// input from previous stage
in VS_OUT
{
    vec4 position;      // position of fragment in model space
    vec3 normal;        // normal vector in model space
    vec3 tangent;       // tangent vector in model space
    vec2 texCoord;      // texture coordinate
} fragment;

float calculateLightAttenFactor()
{
    // location-less (directional) lighting has no attenuation
    if (light.position.w == 0.0)
        return 1.0;

    // check for outside of cone
    if (light.angle > 0.0)
    {
        vec3 L = normalize(
            light.position.xyz - 
            (transforms.mMatrix * fragment.position).xyz
        );    
    
        float lightToSurfaceAngle = degrees(acos(dot(-L, normalize(light.direction))));
        if(lightToSurfaceAngle > light.angle)
            return 0.0;
    }

    vec3 worldPos = (transforms.mMatrix * fragment.position).xyz;
    float distance = distance(light.position.xyz, worldPos.xyz);
    return 1.0 / (1.0 + light.attenuationFactor * pow(distance,2));
}

void main(void)
{
    vec3 N = normalize(mat3(transforms.mvMatrix) * fragment.normal);
    vec3 L = normalize(
        (transforms.vMatrix * vec4(light.position.xyz, 1.0)).xyz - 
        (transforms.mvMatrix * fragment.position).xyz
    );
    vec3 V = normalize(-(transforms.mvMatrix * fragment.position).xyz); 
    
    // location-less (directional) lighting
    if (light.position.w == 0.0)
    {
        L = normalize( (mat3(transforms.vMatrix) * light.direction).xyz );
    }
    
    // recalculate vectors for normal mapping (if enabled)
    if (normalMapping != 0)
    {
        vec3 T = normalize(mat3(transforms.mvMatrix) * fragment.tangent);
        vec3 B = cross(N, T);
    
        L = normalize(vec3(dot(L,T), dot(L,B), dot(L,N)));
        V = normalize(vec3(dot(V,T), dot(V,B), dot(V,N)));
        
        // calculate real normal from normal map
        N = normalize(texture2D(normalMap, fragment.texCoord).rgb * 2.0 - vec3(1.0));
    }
    
    
    float lightFactor = calculateLightAttenFactor() * light.intensity;
    
    vec3 H = normalize(L + V);
    
    vec4 diffuseColor = texture2D(textureMap, fragment.texCoord);
    
    vec3 ambient = diffuseColor.rgb * light.color.rgb * light.ambientFactor * lightFactor;
    vec3 diffuse = max(dot(N,L), 0.0) * diffuseColor.rgb * light.color.rgb * lightFactor;
    vec3 specular = pow(max(dot(N,H), 0.0), material.specularPower) * material.specularColor * 
        light.color.rgb * lightFactor; 
    
    vec3 color = ambient + diffuse + specular;
    gl_FragColor = vec4(pow(color,gammaCorrectionFactor), diffuseColor.a);
}