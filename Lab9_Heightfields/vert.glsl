#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

// We now have our camera system set up.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// We add our sampler here to extract height information
uniform sampler2D tex;

// We define a new output vec2 for our texture coorinates.
out vec2 texCoords;
out vec3 norm;
out vec3 fragPos;

void main()
{   
    // Option 1: CPU heightmapping
    vec4 mappedPos = vec4(position, 1.0);

    // Option 2: GPU heightmapping
    // vec4 heightVec = texture(tex, textureCoords);
    // float heightValue = heightVec.r;
    // float scale = 255.0 * 8.0;
    // float height = heightValue / scale;
    // vec4 mappedPos = vec4(position.x, height, position.z, 1.0);

    // Common code:
    gl_Position = projectionMatrix*viewMatrix*modelMatrix*mappedPos;
    fragPos = (modelMatrix*mappedPos).xyz;
    vec4 tmpnorm = modelMatrix*vec4(normal, 0.0);
    norm = normalize(tmpnorm.xyz);
    texCoords = textureCoords;
}