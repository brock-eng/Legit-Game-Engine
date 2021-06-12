#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out vec4 pos;

void main()
{
   pos = position;
   float intermediate = pr_matrix * vw_matrix * ml_matrix;
   gl_Position = pos * intermediate;
   //gl_Position = pos ;
};

#shader fragment
#version 330 core

uniform vec4 colour;
uniform vec2 light_pos;
uniform float click;

in vec4 pos;

layout(location = 0) out vec4 color;

void main()
{
   float intensity = 1.0 / length(pos.xy - light_pos) * 0.5;
   color = colour * intensity * click;
};

