#version 440 core
layout (location = 0) in uint aVertex;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

flat out uint NormalIndex;
out vec3 UV;

vec2 getUV(uint UVIndex, uint textureIndex)
{
    vec2 base = vec2(0, 0);
    if (UVIndex == 0)   base += vec2(0, 1);
    else if (UVIndex == 1)  base += vec2(1, 1);
    else if (UVIndex == 2)  base += vec2(0, 0);
    else if (UVIndex == 3)  base += vec2(1, 0);

    return base;
}

void main()
{
    uint x = (aVertex & 31);
    uint y = ((aVertex >> 5) & 31);
    uint z = ((aVertex >> 10) & 31);
    uint normalIndex = ((aVertex >> 15) & 7);
    uint UVIndex = ((aVertex >> 18) & 3);
    uint textureIndex = ((aVertex >> 20) & 127);

    vec2 localUV = getUV(UVIndex, textureIndex);
    
    gl_Position = uProjection * uView * uModel * vec4(x, y, z, 1.0);
    NormalIndex = normalIndex;
    UV = vec3(localUV, textureIndex);
}