#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
vec3 lightVec=vec3(3.0,-1.0,1.0);

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

float innerProduct(vec3 a, vec3 b){
	return (a.x*b.x+a.y*b.y+a.z*b.z);
}

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);
	
	//shade depending on angle
	float angle = acos(innerProduct(fragNormal,lightVec)/(length(fragNormal)*length(lightVec)));
	//angle too large - set color to black
	texelColor.xyz*=angle/radians(90.0);
	
	
	
    
    finalColor = texelColor*colDiffuse;
}

