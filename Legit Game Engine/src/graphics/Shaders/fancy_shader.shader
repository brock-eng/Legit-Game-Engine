#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
   vec4 position;
   vec4 color;
} vs_out;

void main()
{
   gl_Position = position;
   vs_out.position = position;
   vs_out.color    = color;
   //gl_Position = pos ;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 light_pos;
uniform float click;

in DATA
{
   vec4 position;
   vec4 color;
} fs_in;

void main()
{
   float intensity = 1.0 / length(fs_in.position.xy - light_pos);
   color = fs_in.color * intensity * click;
};

