#version 330

in VS_OUT {
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

out vec4 fragmentColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main() {
  vec3 objectColor = texture(diffuseMap, fs_in.TexCoords).xyz;
  vec3 normal = texture(normalMap, fs_in.TexCoords).xyz;

  vec3 lightColor = vec3(1.0, 1.0, 1.0);

  // Ambient
  float ambientStrength = 0.2f;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse
  vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 result = (ambient + diffuse) * objectColor;
  fragmentColor = vec4(result, 1.0f);
}