//#version 320
attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;
//in vec4 position;
//in vec4 normal;


varying vec3 mypos;
varying vec3 mynorm;

//out vec3 mypos;
//out vec3 mynorm;

void main()
{
		vec4 transformed;

		//attindex = index;
		
		//mynorm = gl_NormalMatrix*normalvector;
		//mynorm = gl_NormalMatrix*normal.xyz;
		//transformed = gl_ModelViewMatrix * gl_Vertex;
		mynorm = gl_NormalMatrix*normal.xyz;
		transformed = gl_ModelViewMatrix * vec4(position.x,position.y,position.z,1);
		mypos = vec3( transformed.x, transformed.y, transformed.z);
		
		gl_FrontColor = gl_Color;
		
		gl_Position = ftransform();
}
