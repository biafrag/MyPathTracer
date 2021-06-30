/**
 * @brief The vertex shader gives sets the write position of the vertices.
 *
 * @author Bianca Fragoso
 */

#version 330 core

/**
 * @brief The vertex value. It is on the location 0.
 */
layout(location = 0) in vec3 vertexPos;

/**
 * @brief The normal value. It is on the location 1.
 */
layout( location = 1 ) in vec3 vertexNormal;

/**
 * @brief The texture coordinate value. It is on the location 2.
 */
layout( location = 2 ) in vec2 vertexTexCoord;

/**
 * @brief The model-view-projection matrix.
 */
uniform mat4 mvp;

/**
 * @brief The model-view matrix.
 */
uniform mat4 mv;

/**
 * @brief The normal matrix.
 */
uniform mat4 mv_ti;

/**
 * @brief The texture coordinates that will go to the fragment shader.
 */
out vec2 fragUV;

/**
 * @brief The vertex position in eye space that will go to the fragment shader.
 */
out vec3 fragPos;

/**
 * @brief The normal in eye space that will go to the fragment shader.
 */
out vec3 fragNormal;

void main()
{
   gl_Position = mvp * vec4(vertexPos, 1);

   fragPos = ( mv * vec4( vertexPos, 1 ) ).xyz;

   fragNormal = ( mv_ti * vec4( vertexNormal, 0 ) ).xyz;

   fragUV = vertexTexCoord;
}
