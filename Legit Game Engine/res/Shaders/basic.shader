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
   vec4 calc_position = position * pr_matrix * vw_matrix * ml_matrix;
   gl_Position = calc_position;
   vs_out.position = calc_position; 
   vs_out.texCoords = texCoords;
   vs_out.color = color;
   vs_out.textureID = textureID;
};

// ------------------------------------------------------------------------ //

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 incolor;

void main() 
{
   color = incolor;
};

