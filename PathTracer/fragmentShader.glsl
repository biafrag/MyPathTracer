#version 330 core
struct Material
{
    vec3 albedo;
    vec3 specular;
    vec3 color;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

//uniform Light light;
uniform Material material;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragUV;

const int NR_LIGHTS = 1;

uniform Light lights[NR_LIGHTS]; //Vetor de luzes na posição do olho

out vec4 color;
void main()
{
    color = vec4(0, 0, 0, 1);
    for(int i = 0; i < NR_LIGHTS; i++)
    {
        Light light = lights[i];
        vec4 ambient = vec4(light.ambient * material.color, 0.0);
        vec4 diffuse = vec4(0.0);
        vec4 specular = vec4(0.0);

        vec3 N = normalize(fragNormal);
        vec3 L = normalize(light.position - fragPos);

        float iDif = dot(L,N);

        if( iDif > 0 )
        {
            diffuse = iDif * vec4(light.diffuse * material.color, 1);

            vec3 V = normalize(- fragPos);
            vec3 H = normalize(L + V);

            float iSpec = pow(max(dot(N,H),0.0), light.shininess);
            specular = iSpec * vec4(light.specular, 1); // * light.specular;
        }

        color += diffuse + ambient + specular;
    }

}

