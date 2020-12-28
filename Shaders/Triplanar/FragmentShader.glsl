#version 460
#define MAX_LIGHTS 8
out vec4 fragColor;

struct Material {
    bool hasDiffuseTexture;
    sampler2D diffuseTexture;

    bool hasDiffuseColor;
    vec3 diffuseColor;

    bool hasSpecularTexture;
    sampler2D specularTexture;
    float shininess;
    
    bool hasNormalTexture;
    sampler2D normalTexture;    
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 o_worldPos;  
in vec3 o_normal;  
in vec2 o_texCoord;
in vec3 o_cameraPosition;
in vec3 o_tangent;

uniform int u_lightCount;
uniform Material u_material;
uniform Light u_light[MAX_LIGHTS];

vec3 calcNormal() {
    vec3 normal = normalize(o_normal);
    vec3 tangent = normalize(o_tangent);
        //Gram–Schmidt process
    tangent = normalize(tangent - dot(tangent, normal) * normal);       
    vec3 bitangent = cross(tangent, normal);
    vec3 bumpMapNormal = texture(u_material.normalTexture, o_texCoord).xyz;
        //převod z vektoru barvy o rozsahu <0,1> do vektoru normály <-1,1>
    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0); 
        // Transformační  matice TBN
    mat3 TBN = mat3(tangent, bitangent, normal);   
    return normalize(TBN * bumpMapNormal);
}

vec3 getTriPlanarBlend(vec3 _wNorm){
	// in wNorm is the world-space normal of the fragment
	vec3 blending = abs( _wNorm );
	blending = normalize(max(blending, 0.00001)); // Force weights to sum to 1.0
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);
	return blending;
}

vec3 computeLight(Light light){
        //attenuation
        float distance = length(light.position - o_worldPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // ambient
         vec3 ambient = light.ambient ;
  	
        // diffuse 
        vec3 norm;
        if(u_material.hasNormalTexture){
            norm = calcNormal();
        }
        else{
            norm = normalize(o_normal); 
        }
        vec3 lightDir = normalize(light.position - o_worldPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff ;
    
        if(u_material.hasDiffuseTexture){
            diffuse *= texture(u_material.diffuseTexture, o_texCoord).rgb;
            ambient *= texture(u_material.diffuseTexture, o_texCoord).rgb;
        }  
        else if(u_material.hasDiffuseColor){
            diffuse *= u_material.diffuseColor;
            ambient *= u_material.diffuseColor;
        }
        else{
            diffuse *= vec3(102, 153, 204);
            ambient *= vec3(102, 153, 204);
        }
        // specular
        vec3 viewDir = normalize(o_cameraPosition - o_worldPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = light.specular * spec;

    //result
        return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 res = vec3(0.f);
    for(int i = 0; i < u_lightCount; i++ ){
        res += computeLight(u_light[i]);
    }   
    fragColor = vec4(res, 1.0);
}