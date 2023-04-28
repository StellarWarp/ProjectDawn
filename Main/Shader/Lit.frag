#version 330 core

out vec4 FragColor;

in v2f{
	vec3 pos_world;
	vec3 normal_world;
	vec4 pos_light_view;
}i;
//in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform vec3 light_dir;

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	
 // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;

	vec3 normal = normalize(i.normal_world);
    vec3 lightDir = light_dir;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.00005);

	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    float shadow_factor = 1 - shadow;

    return shadow_factor;
} 

void main()
{
	vec3 object_color = vec3(1,1,1);
	vec3 light_color = vec3(1.0, 1.0, 1.0);
//    vec3 light_dir = normalize(vec3(1.0, 1.0, 1.0));
	float diff = max(dot(i.normal_world, light_dir), 0.0);
	vec3 diffuse = diff * light_color;
	vec3 ambient = vec3(0.1, 0.1, 0.1);

    float shadow = ShadowCalculation(i.pos_light_view);
	vec3 lighting = (diffuse * shadow  + ambient) * object_color;
//	vec3 lighting = shadow * object_color;

    FragColor = vec4(lighting,1);
}     