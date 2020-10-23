#version 330 core  

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

in vec3 Normal;
in vec3 FragPosition;

out vec4 FragColor;  

const int Shininess = 32;
const vec3 SpecularStrength = 1.0 * vec3(1.0);


void main()  
{  
   vec3 lightDirection = normalize(lightPosition - FragPosition);

   float ambientLightStrength = 0.2f;
   vec3 ambientLight = lightColor * ambientLightStrength;

   vec3 diffuseLight = max(dot(Normal, lightDirection), 0.0) * lightColor;

   
   vec3 dirFromFragPosToCam = normalize(cameraPosition - FragPosition);
   vec3 lightReflectedAboutNormal = reflect(-lightDirection, Normal);
   float spec = clamp(dot(lightReflectedAboutNormal, dirFromFragPosToCam), 0.0, 1.0);
   vec3 specularLight = SpecularStrength * pow(spec, Shininess);

   vec3 resultingLight = objectColor * (ambientLight + diffuseLight + specularLight);

   FragColor = vec4(resultingLight, 1.0);
} ;