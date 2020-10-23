#version 330 core  

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPosition;

in vec3 Normal;
in vec3 FragPosition;

out vec4 FragColor;  


void main()  
{  
   vec3 lightDirection = normalize(lightPosition - FragPosition);

   float ambientLightStrength = 0.2f;
   vec3 ambientLight = lightColor * ambientLightStrength;

   vec3 diffuseLight = max(dot(Normal, lightDirection), 0.0) * lightColor;

   vec3 resultingLight = objectColor * (ambientLight + diffuseLight);

   FragColor = vec4(resultingLight, 1.0);
} ;