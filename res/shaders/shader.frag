#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
}
fs_in;

uniform vec4 color;
uniform vec3 lightPos; // In world space
uniform sampler2D ourTexture;

void main()
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * color.rgb;

    // diffuse
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color.rgb;

    vec3 result = ambient + diffuse;
    FragColor = texture(ourTexture, fs_in.TexCoords);
}