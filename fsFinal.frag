#version 330 core
out vec4 FragColor;  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec4 MVMat;
in vec3 Normal;

uniform sampler2D texture_diffuse1;

float getFogFactor(float fogCoordinate)
{
	return 1.0 - clamp(exp(-0.005 * fogCoordinate), 0.0, 1.0);
}

void main()
{
    // ambient
    float ambientStrength = 0.9;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);

    vec3 brightLightColor = lightColor * 1.3;  
    if (lightColor == vec3(1.0, 1.0, 1.0))
    {
        brightLightColor = lightColor;
    }

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 lightDir = normalize(-lightPos);

	float diff = max(dot(norm, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);

	vec3 diffuse = diff * brightLightColor;
	vec3 specular = specularStrength * spec * brightLightColor;
	
    vec3 result = (ambient + diffuse + specular) * vec3(texture(texture_diffuse1, TexCoords));
    FragColor = vec4(result, texture(texture_diffuse1, TexCoords).a);

	float fogCoordinate = abs(MVMat.z / MVMat.w);
	FragColor = mix(FragColor, vec4(0.2, 0.28, 0.29, texture(texture_diffuse1, TexCoords).a), getFogFactor(fogCoordinate));
}
