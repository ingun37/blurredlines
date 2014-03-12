attribute vec3 normalvector;
varying vec3 mynorm;
varying vec3 mypos;
void main()
{
		vec4 transformed;
		mynorm = gl_NormalMatrix*normalvector;
		transformed = gl_ModelViewMatrix * gl_Vertex;
		mypos = vec3( transformed.x, transformed.y, transformed.z);
		
		gl_FrontColor = gl_Color;
		
		gl_Position = ftransform();
}
