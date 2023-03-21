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
    // 检查当前片段是否在阴影中
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return 1;
}

void main()
{
	vec3 object_color = vec3(1,1,1);
	vec3 light_color = vec3(1.0, 1.0, 1.0);
    vec3 light_dir = normalize(vec3(1.0, 1.0, 1.0));
	float diff = max(dot(i.normal_world, light_dir), 0.0);
	vec3 diffuse = diff * light_color;
	vec3 ambient = vec3(0.1, 0.1, 0.1);

    float shadow = ShadowCalculation(i.pos_light_view);
	vec3 lighting = (diffuse * shadow  + ambient) * object_color;

    FragColor = vec4(lighting,1); 
}