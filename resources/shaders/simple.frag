out vec4 FragColor;

in vec3 fragPos;
in vec2 texCoords;

uniform bool useTexture;
layout(binding = 0) uniform sampler2D tex; 

void main()
{    
   if (useTexture) {
      vec3 color = texture(tex, texCoords).rgb;
      FragColor = vec4(color, 1.0);
   } else {

      FragColor = vec4(fragPos, 1.0);
   }
}