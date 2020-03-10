#version 330

in vec2 textureCoordinates;

out vec4 fragmentColor;

uniform sampler2D tex;

void main() {
  // Set our output fragment color to whatever we pull from our input texture (Note, change 'tex' to whatever the sampler is named)
  fragmentColor = texture(tex, textureCoordinates);
}