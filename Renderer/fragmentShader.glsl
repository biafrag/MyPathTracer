/**
 * @brief The fragment shader gives the color to the fragments.
 *
 * @author Bianca Fragoso
 */
#version 330 core

/**
 * @brief The material with its properties.
 */
struct Material
{
    /**
     * @brief The material color.
     */
    vec3 albedo;

    /**
     * @brief The reflection color.
     */
    vec3 specular;
};

/**
 * @brief The light with its properties.
 */
struct Light
{
    /**
     * @brief The light's position.
     */
    vec3 position;

    /**
     * @brief The light's ambient component.
     */
    vec3 ambient;

    /**
     * @brief The light's diffuse component.
     */
    vec3 diffuse;

    /**
     * @brief The light's specular component.
     */
    vec3 specular;

    /**
     * @brief The light's shininess.
     */
    float shininess;
};

/**
 * @brief The fragment's material.
 */
uniform Material material;

/**
 * @brief The boolean indicating if there is texture or not.
 */
uniform int hasTexture;

/**
 * @brief The fragment position in eye space.
 */
in vec3 fragPos;

/**
 * @brief The normal in eye space.
 */
in vec3 fragNormal;

/**
 * @brief The texture coordinate of the fragment.
 */
in vec2 fragUV;

/**
 * @brief The sampler with the texture.
 */
uniform sampler2D sampler;

/**
 * @brief The number of lights.
 */
const int NR_LIGHTS = 2;

/**
 * @brief The lights.
 */
uniform Light lights[NR_LIGHTS];

/**
 * @brief The fragment's final color.
 */
out vec4 color;

void main()
{
    color = vec4(0, 0, 0, 1);
    vec3 matColor;
    if(hasTexture == 0)
    {
        matColor = material.albedo;
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
            specular = iSpec * vec4(light.specular, 1);
        }

        color += diffuse + ambient + specular;
    }

}

