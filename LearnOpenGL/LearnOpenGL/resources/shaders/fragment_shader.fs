#version 330 core  

struct Material {
   vec3 ambientStrength;
   vec3 diffuseStrength;
   vec3 specularStrength;
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


in vec3 Normal;
in vec3 FragPosition;

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
   return light.ambientStrength * material.ambientStrength;
};

vec3 calculateDiffuseLight(vec3 lightDirection ) {
   float diff = max( dot( normalize(Normal), lightDirection), 0.0);
   return diff * light.diffuseStrength * material.diffuseStrength;
};

vec3 calculateSpecularLight(vec3 lightDirection) {
   vec3 dirFromFragPosToCam = normalize(cameraPosition - FragPosition);
   vec3 lightReflectedAboutNormal = reflect(-lightDirection, normalize(Normal));
   float spec = pow( max( dot(lightReflectedAboutNormal, dirFromFragPosToCam), 0.0), material.shininess);
   return material.specularStrength *  light.specularStrength  ;
};

