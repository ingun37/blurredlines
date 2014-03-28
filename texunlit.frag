varying vec2 myuv;
varying vec3 mynorm;

uniform sampler2D tex;

void main()
{
		gl_FragColor = texture2D(tex, myuv);
		
}
