#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gColor;
layout (location = 2) out vec4 gTangent;
layout (location = 3) out vec4 gBitangent;
layout (location = 4) out vec4 gNormal;

in VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;

    vec3 T, N, B; // Tangent, Normal, Bitangent
}
fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;
uniform sampler2D roughnessMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 

    // scale factor for height map
    float heightScale = 0.25; // adjust this value to control the parallax

    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));

    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = 1.0 - texture(heightMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = 1.0 - texture(heightMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = (1.0 - texture(heightMap, prevTexCoords).r) - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{

    mat3 TBN = transpose(mat3(fs_in.T, fs_in.B, fs_in.N));

    vec3 TangentViewPos  = TBN * viewPos;
    vec3 TangentFragPos  = TBN * fs_in.FragPos;

    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

    vec2 texCoords = ParallaxMapping(fs_in.TexCoords / 1.0,  viewDir);

    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    vec3 color = texture(diffuseMap, texCoords).rgb;

    gPosition = vec4(fs_in.FragPos, 1.0);
    gColor = vec4(color, 1.0);
    gTangent = vec4(fs_in.T, 1.0);
    gBitangent = vec4(fs_in.B, 1.0);
    gNormal = vec4(normal, 1.0);

}