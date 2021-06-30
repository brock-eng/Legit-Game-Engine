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
   gl_Position = calc_position;// *vw_matrix* ml_matrix;
   vs_out.position = calc_position; // *pr_matrix;
   vs_out.texCoords = texCoords;
   vs_out.color = color;
   vs_out.textureID = textureID;
   //gl_Position = pos ;
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
uniform int numIterations;

precision highp float;
uniform mat4 uMVPMatrix;// = mat4(1.0);

void main() {

	vec2 p = (uMVPMatrix * vec4(fs_in.position.x, fs_in.position.y, 0, 1)).xy;
	vec2 c = p;

	vec3 color = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < numIterations; i++) {
		p = vec2(p.x * p.x - p.y * p.y, 2.0 * p.x * p.y) + c;
		if (dot(p, p) > 4.0) {
			float colorRegulator = float(i - 1) - log(((log(dot(p, p))) / log(2.0))) / log(2.0);
			color = vec3(0.95 + .012 * colorRegulator, 1.0, .2 + .4 * (1.0 + sin(.3 * colorRegulator)));
			break;
		}
	}
	// change color from HSV to RGB. Found: https://gist.github.com/patriciogonzalezvivo/114c1653de9e3da6e1e3
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 m = abs(fract(color.xxx + K.xyz) * 6.0 - K.www);
	gl_FragColor.rgb = color.z * mix(K.xxx, clamp(m - K.xxx, 0.0, 1.0), color.y);

	gl_FragColor.a = 1.0;
}

