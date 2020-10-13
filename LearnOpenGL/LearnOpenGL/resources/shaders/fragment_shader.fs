#version 330 core  
out vec4 FragColor;  
in vec3 color;  
in vec2 texCoord;
in vec3 position;

uniform sampler2D textureSampler; 
void main()  
{  
   // FragColor = texture(textureSampler, texCoord);
   FragColor = normalize(gl_FragCoord);
} ;