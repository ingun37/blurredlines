varying vec3 mypos;
varying vec3 mynorm;
uniform vec3 lightdir;
uniform vec3 Lirradiance;
void main()
{
		float normlen;
		vec3 normednorm;
		float cosnl;
		vec3 Lcos;
		vec3 Ldiff;
		normlen = length(mynorm);
		normednorm = vec3(mynorm.x/normlen, mynorm.y/normlen, mynorm.z/normlen);
		cosnl = dot(lightdir,normednorm);
		Lcos = vec3(Lirradiance.x * cosnl,Lirradiance.y * cosnl,Lirradiance.z * cosnl);
		Ldiff = vec3(gl_Color.x * Lcos.x/3.141592,gl_Color.y * Lcos.y/3.141592,gl_Color.z * Lcos.z/3.141592);
		
		gl_FragColor = vec4(Ldiff.x,Ldiff.y,Ldiff.z,1);

}
