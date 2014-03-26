#ifndef materialProperties_h
#define materialProperties_h
typedef struct MaterialPropertiesraw
{
		float diffuse[4];
		float ambient[4];
		float specular[4];
		float shininess;
}MaterialProperties;
#endif