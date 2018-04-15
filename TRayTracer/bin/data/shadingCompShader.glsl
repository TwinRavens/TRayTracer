#version 430

layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

uniform float time;
uniform int spheresCount;
uniform vec4 ambientColour;

struct Ray
{
	vec4 origin;
	vec4 dirAndId;
};

layout (std140) buffer rBuffer
{
  Ray rays[];
} RaysBlock;

struct Spheres
{
	vec4 posAndScale;
	vec4 diffColAndCoef;
	vec2 specPowAndCoef;
};

layout (std140) buffer sBuffer
{
	Spheres spheres[];
} SpheresBlock;

//define light
vec3 lightPos = vec3(0.0, 0.0, 0.0);
vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

vec4 shade(vec3 point, vec3 normal, vec3 viewDir) {

		//ambient
		float ka = 0.2;  //coeficient of ambient light
		vec4 color = vec4(1.0, 1.0, 1.0, 1.0); //color of ambient light
		vec4 ia = color * ka;


		//diffuse
		float kd = 0.8; // coeficient of diffuse light
		vec4 diffuseColor = vec4(1.0, 0.0, 0.0, 1.0); // color from the object

		vec3 lightDir = normalize(lightPos - point);
		float diffuse = clamp(dot(lightDir, normal), 0, 1);
		vec4 id = kd * diffuse * diffuseColor * lightColor;

		//specular TODO:review this
		float ks = 0.6;
		float specullarPower = 50;

		//Reflect = lightDir - 2 * normal * dot(normal, lightDir)
		//Ou normalize(2 * normal * dot(normal, lightDir) - lightDir) quando o raio vem da fonte de luz
		vec3 reflect = normalize(lightDir - 2 * normal * diffuse);
		float specular = clamp(abs(pow(dot(reflect, viewDir), specullarPower)), 0, 1);
		
		vec4 is = ks * specular * lightColor;

		return (ia + id + is);
}

void main() {
	// base pixel colour for image
	vec4 pixel = vec4(0.0f, 0.4509803921568627f, 0.8980392156862745f, 1.0);

	// get index in global work group i.e x,y position
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  
	//view size
	float max_x = 16.0;
	float max_y = 9.0;

	ivec2 dims = imageSize(img_output); // fetch image dimensions

	//Get an X and Y ray position varying from [-1->1]
	float x = (float(pixel_coords.x * 2 - dims.x) / dims.x);
	float y = (float(pixel_coords.y * 2 - dims.y) / dims.y);

	//Get ray id
	int rayId = (pixel_coords.x+pixel_coords.y*dims.x);

	//define camera points
	vec3 cameraOrigin = vec3(RaysBlock.rays[rayId].origin.x * max_x, RaysBlock.rays[rayId].origin.y * max_y, 0.0);
	vec3 cameraDir = RaysBlock.rays[rayId].dirAndId.xyz; // ortho

	int indexClosestSphere = -1;
	float closestDelta = 4294967296.0; 

	for(int i = 0; i < spheresCount; i++)
	{
		//define sphere
		vec3 sphereCenter = SpheresBlock.spheres[i].posAndScale.xyz;
		//vec3 sphereCenter = vec3(i * 5 - 15, sin(time + i*0.5f) * 3, -10.0);
		float sphereRadius = SpheresBlock.spheres[i].posAndScale.w;
		//float sphereRadius = 1;

		//region collision with sphere
		vec3 omc = cameraOrigin - sphereCenter; 
		float b = dot(cameraDir, omc);
		float c = dot(omc, omc) - sphereRadius * sphereRadius;
		float delta = b * b - c;

		// hit one or both sides
		if (delta >= 0.0) {
			float hitT = -b - delta;
			if(hitT < closestDelta && hitT > 1) {
				indexClosestSphere = i;
				closestDelta = hitT;
			}
		}
	}

	if(indexClosestSphere != -1) {
		vec3 sphereCenter = SpheresBlock.spheres[indexClosestSphere].posAndScale.xyz;
		//vec3 sphereCenter = vec3(indexClosestSphere * 5 - 15, sin(time + indexClosestSphere*0.5f) * 3, -10.0);
		float sphereRadius = 1.0;

		vec3 hit = vec3(cameraOrigin.x + cameraDir.x * closestDelta, cameraOrigin.y + cameraDir.y * closestDelta, cameraOrigin.z + cameraDir.z * closestDelta);
		vec3 normal = vec3((hit.x - sphereCenter.x)/sphereRadius,  (hit.y - sphereCenter.y)/sphereRadius, (hit.z - sphereCenter.z)/sphereRadius); 

		pixel = shade(hit, normal, cameraDir);
	}

	// output to a specific pixel in the image
	imageStore(img_output, pixel_coords, pixel);
}