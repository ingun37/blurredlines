varying vec2 myuv;
varying vec3 mynorm;

void main()
{
		vec4 color = vec4(1,1,1,1);
		
		if( mod(floor(myuv.x/0.1) + floor(myuv.y/0.1), 2.0)  ==1.0)
		{
				color = vec4(0,0,0,1);
		}
		
		gl_FragColor = color;
		
}
