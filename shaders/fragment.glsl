#version 440 core
out vec4 FragColor;

flat in uint NormalIndex;
in vec3 UV;

uniform sampler2DArray TextureArray;

vec3 normals[] = {
    vec3(-1.0, 0.0, 0.0),
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, -1.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, -1.0),
    vec3(0.0, 0.0, 1.0)
};

void main()
{
    const vec3 lightDir = normalize(vec3(-0.2, 1.0, -0.7));
    const vec3 lightColor = vec3(1.0);
    const float ambientStrength = 0.4;

    vec3 normal = normalize(normals[NormalIndex]);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;

    vec3 texColor = texture(TextureArray, UV).rgb;

    vec3 result = (ambient + diffuse) * texColor;
    
    FragColor = vec4(result = 1.0);
}