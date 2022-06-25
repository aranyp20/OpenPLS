#shader vertex
#version 330 core

struct Light
{
	vec3 La, Le;
	vec4 wLightPos;
};

struct Material
{
	vec3 kd, ks, ka;
	float shininess;
};

uniform mat4 M, V, P, Minv;
uniform vec3 wEye;
uniform Light light;
uniform Material material;

layout(location = 0) in vec3 mVtxPos;
layout(location = 1) in vec3 mVtxNorm;

out vec3 radiance;

//Calculate radiance in every vertex, interpolate it to every fragment
void main()
{



	vec4 mPos = vec4(mVtxPos, 1);
	vec4 mNorm = vec4(mVtxNorm, 0);
	//Calculating normalized position of current vertex
	gl_Position = mPos * M * V * P;
	//Calculating radiance of current vertex
	vec4 wPos = mPos * M;
	vec3 V = normalize(wEye * wPos.w - wPos.xyz);
	vec3 N = normalize((Minv * mNorm).xyz);
	if (dot(N, V) < 0) {
		N = -N;
		
	}
	vec3 L = normalize(light.wLightPos.xyz * wPos.w - wPos.xyz * light.wLightPos.w);
	vec3 H = normalize(L + V);
	//ignore spec component
	radiance = material.ka * light.La + light.Le * (material.kd * max(dot(N, L), 0));

}

#shader fragment
#version 330 core

in vec3 radiance;
out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(radiance, 1);

}

