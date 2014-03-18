varying vec3 mypos;//월드행렬이 곱해진 버텍스의 위치 interpolation된값
varying vec3 mynorm;//노말 interpolation된
uniform vec3 eyepos;//눈위치
uniform vec3 specc;
uniform vec3 spherecenter;

//////일직선 빛 정보들
uniform vec3 lightdir;//빛의 방향 (빛이오는쪽방향)
uniform vec3 Lirradiance;//빛의 밝기 ( rgb값)
//////


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
		vec3 finalspeccolor;
		
		vec3 finalcolor;
		vec3 ambient;
		
		//mynorm은 normalize된 normal벡터가 아니므로 다시 해준다.
		normednorm = normalize(mynorm);
		//clamp해주는 이유는 cos값이 0보다 작아지면 표면이 빛을 받지않는 쪽을 바라보고있다는뜻이므로 그냥 0으로 해준다.
		
		cosnl = clamp( dot(lightdir,normednorm),0.0,1.0);
		
		//빛의 밝기에 cos값을 곱해서 경사가 질수록 밝기가 줄어드는 부분.
		Lcos = cosnl * Lirradiance;
		//diffuse light에 곱해줄 상수
		//diffuse color로는 그냥 gl_Color를 쓴다.
		diffIntense = 1.0/3.141592;
		//diffIntense = 0.0;
		finaldiffcolor = diffIntense * gl_Color;
		//specular light에 곱해줄 상수 M 클수록 빛나는부분이 좁아진다.
		constM = 20.0;
		//mypos -> eyepos 벡터 normalize
		view = eyepos - mypos;
		view = normalize(view);
		
		//view와 light의 half벡터 normalize
		myhalf = view + lightdir;
		myhalf = normalize(myhalf);
		
		//half벡터와 normal벡터가 비슷할수록 눈위치가 빛방향의 반사방향과 비슷하다는뜻이므로 빛나는 부분.
		coshn = dot(normednorm,myhalf);
		specIntense = (constM/7.0 + 1.0)/8.0*3.141592*pow(coshn,constM);
		finalspeccolor = specIntense * specc;
		
		finalcolor = (Lcos * (finaldiffcolor.xyz + finalspeccolor.xyz)) + (0.1 * gl_Color.xyz);
		
		gl_FragColor = vec4(finalcolor.x,finalcolor.y,finalcolor.z,1);

}
