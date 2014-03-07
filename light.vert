attribute vec3 normalvector;
varying vec3 mynorm;
varying vec3 mypos;
void main()
{
		mypos = vec3( gl_Vertex.x, gl_Vertex.y, gl_Vertex.z);
		mynorm = gl_NormalMatrix*normalvector;
		gl_FrontColor = gl_Color;
		gl_Position = ftransform();
}
