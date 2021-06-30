/**
 * @brief The fragment shader gives the color to the fragments.
 *
 * @author Bianca Fragoso
 */
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

uniform Material material;
uniform int hasTexture;
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragUV;
uniform sampler2D sampler;

const int NR_LIGHTS = 2;

uniform Light lights[NR_LIGHTS];

out vec4 color;
void main()
{
    color = vec4(0, 0, 0, 1);
    vec3 matColor;
    if(hasTexture == 0)
    {
        matColor = material.color;
    }
    else
    {
        matColor = texture(sampler, fragUV).rgb;
    }

    for(int i = 0; i < NR_LIGHTS; i++)
    {
        Light light = lights[i];
        vec4 ambient = vec4(light.ambient * matColor, 0.0);
        vec4 diffuse = vec4(0.0);
        vec4 specular = vec4(0.0);

        vec3 N = normalize(fragNormal);
        vec3 L = normalize(light.position - fragPos);

        float iDif = dot(L,N);

        if( iDif > 0 )
        {
            diffuse = iDif * vec4(light.diffuse * matColor, 1);

            vec3 V = normalize(- fragPos);
            vec3 H = normalize(L + V);

            float iSpec = pow(max(dot(N,H),0.0), light.shininess);
            specular = iSpec * vec4(light.specular, 1); // * light.specular;
        }

        color += diffuse + ambient + specular;
    }

}

