varying vec3 mypos;
uniform vec3 start;
uniform vec3 dir;
uniform float dist;
uniform vec3 mycolor;
uniform float mypow;
void main()
{
//	gl_FragColor = gl_Color;
//gl_FragColor = vec4(0.1,0.2,0.7,0.2);//((dist-ftmp)/dist));
		
	vec3 vtmp = vec3(mypos.x - start.x,mypos.y - start.y,mypos.z-start.z);
	float ftmp = vtmp.x*dir.x + vtmp.y*dir.y + vtmp.z*dir.z;
	vec3 vtmp2 = vec3(dir.x * ftmp + start.x, dir.y * ftmp + start.y, dir.z * ftmp + start.z);
	ftmp = sqrt(
		((vtmp2.x-mypos.x)*(vtmp2.x-mypos.x))+
		((vtmp2.y-mypos.y)*(vtmp2.y-mypos.y))+
		((vtmp2.z-mypos.z)*(vtmp2.z-mypos.z))
		);
/*
	gl_FragColor = vec4(0.7,0.2,
		((mypos.y-start.y)*(mypos.y-start.y))/(dist*dist)
		,1);//((dist-ftmp)/dist));
		*/
		
	gl_FragColor = vec4(mycolor.x,mycolor.y,
		mycolor.z
		,(pow(dist-ftmp,mypow)/pow(dist*dist,mypow)));
}

