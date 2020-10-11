#version 330 core  
layout (location = 0) in vec3 aPos;  
layout (location = 1) in vec3 aColor;  
layout (location = 2) in vec2 aTexCoord;  

uniform mat4 transform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform; 

out vec3 color;  
out vec2 texCoord;  
void main()  
{  
   gl_Position = projectionTransform * viewTransform * transform * vec4(aPos, 1.0f);  
   color = aColor;  
   texCoord = aTexCoord;  
};
