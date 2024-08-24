#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;

// 方向光
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform float lightIntensity;

void main()
{
    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir);  // 光的方向是从片段指向光源
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity;

    // 合并结果
    vec3 result = (ambient + diffuse) * vec3(texture(texture1, TexCoord));
    FragColor = vec4(result, 1.0);
}