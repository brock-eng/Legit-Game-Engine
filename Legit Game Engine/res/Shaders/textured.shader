#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec4 color;
layout(location = 3) in float textureID;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
   vec4 position;
   vec2 texCoords;
   vec4 color;
   float textureID;
} vs_out;

void main()
{
   vec4 calc_position = position *  pr_matrix * vw_matrix * ml_matrix;
   gl_Position = calc_position;
   vs_out.position = calc_position; 
   vs_out.texCoords = texCoords;
   vs_out.color = color;
   vs_out.textureID = textureID;
};

/* --------------------------------------------------------------- */


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in DATA
{
   vec4 position;
   vec2 texCoords;
   vec4 color;
   float textureID;
} fs_in;

uniform vec4 colour;
uniform vec2 light_pos = { 0.0f, 0.0f };
uniform float light_level = 1.0f;
uniform int dynamic_lighting = 0;
uniform sampler2D textures[32];

void main()
{
   float intensity = 1.0 / length(fs_in.position.xy - light_pos);
   int textureCurr = int(fs_in.textureID);
   float noTexture = 0.0f;
   if (dynamic_lighting == 1)
   {
      if (fs_in.textureID == noTexture)
         color = fs_in.color * intensity * light_level;
      else
      {
         color = texture(textures[textureCurr], fs_in.texCoords) * intensity * light_level;
         if (color == 0.0f) discard;
      }
   }
   else
   {
      if (fs_in.textureID == noTexture)
         color = fs_in.color;
      else
      {
         color = texture(textures[textureCurr], fs_in.texCoords);
         if (color == 0.0f) discard;
      }
   }
};

