#version 330 core  

struct Material {
   sampler2D diffuseStrength;
   sampler2D specularStrength;
   float shininess;
};

struct Light {
   vec3 ambientStrength;
   vec3 diffuseStrength;
   vec3 specularStrength;

   vec3 position;
};

uniform Material material;

uniform Light light;

uniform vec3 objectColor;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

// uniform sampler2D texture_diffuse1;
// uniform sampler2D texture_diffuse2;
// uniform sampler2D texture_diffuse3;
// uniform sampler2D texture_diffuse4;
// uniform sampler2D texture_diffuse5;
// uniform sampler2D texture_diffuse6;
// uniform sampler2D texture_diffuse7;
// uniform sampler2D texture_diffuse8;


// uniform sampler2D texture_specular1;
// uniform sampler2D texture_specular2;
// uniform sampler2D texture_specular3;
// uniform sampler2D texture_specular4;
// uniform sampler2D texture_specular5;
// uniform sampler2D texture_specular6;
// uniform sampler2D texture_specular7;
// uniform sampler2D texture_specular8;


in vec3 Normal;
in vec3 FragPosition;
in vec2 TexCoords;

out vec4 FragColor;  



vec3 calculateAmbientLight();
vec3 calculateDiffuseLight(vec3 lightDirection);
vec3 calculateSpecularLight(vec3 lightDirection);


void main()  
{  

   vec3 ambientLight = calculateAmbientLight();


   vec3 lightDirection = normalize(lightPosition - FragPosition);

   vec3 diffuseLight = calculateDiffuseLight(lightDirection);

   vec3 specularLight = calculateSpecularLight(lightDirection);

   vec3 resultingLight = (ambientLight + diffuseLight + specularLight);

   FragColor = vec4(resultingLight, 1.0);
};


vec3 calculateAmbientLight() {
   return light.ambientStrength * vec3(texture(material.diffuseStrength, TexCoords));
};

vec3 calculateDiffuseLight(vec3 lightDirection ) {
   float diff = max( dot( normalize(Normal), lightDirection), 0.0);
   return light.diffuseStrength * diff * vec3(texture(material.diffuseStrength, TexCoords));
};

vec3 calculateSpecularLight(vec3 lightDirection) {
   vec3 dirFromFragPosToCam = normalize(cameraPosition - FragPosition);
   vec3 lightReflectedAboutNormal = reflect(-lightDirection, normalize(Normal));
   float spec = pow( max( dot(lightReflectedAboutNormal, dirFromFragPosToCam), 0.0), material.shininess);
   return light.specularStrength * spec * vec3(texture(material.specularStrength, TexCoords));
};

