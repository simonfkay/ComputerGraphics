#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec3 normal;

// We now have our camera system set up.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 texCoords;
out vec3 normalCoords;
out vec3 FragPos;

void main()
{
    // We have our transformed position set properly now
    gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4(position, 1.0);

    texCoords = textureCoordinates;
    normalCoords = normal;
    FragPos = vec3(modelMatrix * vec4(position, 1.0f));
}