#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform vec3 LightPos;
uniform vec3 ViewPos;

uniform float p;

void main()
{
    // ambient
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vec3 ambient = 0.3f * color.rgb;
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = 0.6f * max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * color.rgb;
    // specular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 halfAngle = normalize(viewDir + lightDir);
    float spec = 3.0f * pow(max(dot(norm, halfAngle), 0.0f), p);
    vec3 specular = spec * color.rgb;
    
    color = vec4(ambient + diffuse + specular , 1.0f);
}

