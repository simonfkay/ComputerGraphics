#version 330

in vec3 FragPos;
in vec2 texCoords;
in vec3 normalCoords;

out vec4 fragmentColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main() {
  vec3 objectColor = texture(diffuseMap, texCoords).xyz;
  vec3 normal = texture(normalMap, texCoords).xyz;

  fragmentColor = vec4(objectColor, 1.0f);



  // vec3 lightColor = vec3(1.0, 1.0, 1.0);
  // vec3 lightPos = vec3(0.0, 8.0, 2.0);

  // // Ambient
  // float ambientStrength = 0.6f;
  // vec3 ambient = ambientStrength * lightColor;

  // // Diffuse
  // vec3 normal = normalize(normalCoords);
  // vec3 lightDir = normalize(lightPos - FragPos);
  // float diff = max(dot(normal, lightDir), 0.0);
  // vec3 diffuse = diff * lightColor;

  // vec3 result = (ambient + diffuse) * objectColor;
  // fragmentColor = vec4(result, 1.0f);
}