//#version 320
varying vec3 mypos;
varying vec3 mynorm;
//int vec3 mypos;
//in vec3 mynorm;

uniform vec3 eyepos;

void main()
{
		vec3 normednorm;
		float cosnl;
		vec3 Lcos;
		float diffIntense;
		vec4 finaldiffcolor;

		vec3 view;
		vec3 myhalf;
		float myhalflen;

		float constM;
		float coshn;
		float specIntense;
		vec4 finalspeccolor;
		
		vec4 finalambcolor;
		
		vec4 finalcolor;
		vec3 ambient;
		

		normednorm = normalize(mynorm);

		
		cosnl = clamp( dot(gl_LightSource[0].spotDirection,normednorm),0.0,1.0);
		
		diffIntense = 1.0/3.141592;
		finaldiffcolor = diffIntense * (gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse);

		constM = 20.0;

		view = eyepos - mypos;
		view = normalize(view);
		
		myhalf = view + gl_LightSource[0].spotDirection;
		myhalf = normalize(myhalf);
		
		coshn = dot(normednorm,myhalf);
		specIntense = (constM/7.0 + 1.0)/8.0*3.141592*pow(coshn,gl_FrontMaterial.shininess);
		finalspeccolor = specIntense * (gl_FrontMaterial.specular * gl_LightSource[0].specular);
		
		finalambcolor = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
		
		finalcolor = (cosnl	* (finaldiffcolor + finalspeccolor)) + finalambcolor;
		
		gl_FragColor = vec4(finalcolor.x,finalcolor.y,finalcolor.z,1);

}
