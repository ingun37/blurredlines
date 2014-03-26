
attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

varying vec2 myuv;
varying vec3 mynorm;

void main()
{
		vec4 transformed;
		
		mynorm = gl_NormalMatrix*normal.xyz;
		myuv = uv;
		
		gl_FrontColor = gl_Color;
		
		gl_Position = ftransform();
}
