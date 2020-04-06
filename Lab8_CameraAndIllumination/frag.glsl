#version 330

// Take in our texture coordinate from our vertex shader
in vec2 texCoords;
// And our normal
in vec3 norm;
// And our fragment position for lighting
in vec3 fragPos;

// We always define a fragment color that we output.
out vec4 FragColor;

// Define our light(s)
struct PointLight {
    vec3 color;
    vec3 position;
    float ambientIntensity;
    float specularIntensity;

    float constant;
    float linear;
    float quadratic;
};

// Maintain our uniforms.
uniform sampler2D tex;              // our primary texture
uniform mat4 view;                  // we need the view matrix for highlights
uniform PointLight pointLights[3];  // Our lights

vec3 calculateLight(int index) {
  vec3 normal = normalize(norm);

  vec3 ambient = pointLights[0].ambientIntensity * pointLights[0].color;

  vec3 lightDir = normalize(pointLights[index].position - fragPos);
  float diffImpact = max(dot(normal, lightDir), 0.0);
  vec3 diffuseLight = diffImpact * pointLights[index].color;

  vec3 viewPos = vec3(0.0, 0.0, 0.0);
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = pointLights[index].specularIntensity * spec * pointLights[index].color;

  return ambient + diffuseLight + specular;
}

void main() {
  vec3 diffuseColor;
  diffuseColor = texture(tex, texCoords).rgb;

  vec3 Lighting = vec3(0.0, 0.0, 0.0);
  for (int ii = 0; ii < 3; ++ii) {
    Lighting += calculateLight(ii);
  }
  
  vec3 finalColor = diffuseColor * Lighting;
  if (gl_FrontFacing) {
    FragColor = vec4(finalColor, 1.0);
  } else {
    // Additionally color the back side the same color
    FragColor = vec4(finalColor, 1.0);
  }
}
