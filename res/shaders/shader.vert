#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;

    vec3 T, N, B; // Tangent, Normal, Bitangent
}
vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    vs_out.FragPos = vec3(model * vec4(aPosition, 1.0));   
    vs_out.TexCoords = aTexCoords;

    vs_out.T = T;
    vs_out.N = N;
    vs_out.B = B;
    

    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
