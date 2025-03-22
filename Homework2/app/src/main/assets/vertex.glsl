#version 300 es

uniform vec3 eyePos;
uniform vec3 lightDir;

uniform mat4 worldMat, viewMat, projMat;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;

//out vec3 v_view, v_light, v_normal;
out vec3 v_lightTS, v_viewTS;   // vec in tan space
out vec2 v_texCoord;

void main() {

    vec3 worldPos = (worldMat * vec4(position, 1.0)).xyz;

    //v_normal = normalize(transpose(inverse(mat3(worldMat))) * normal);

    // calcuate vectors in tangent space
    vec3 Nor = normalize(transpose(inverse(mat3(worldMat))) * normal);
    vec3 Tan = normalize(transpose(inverse(mat3(worldMat))) * tangent);
    vec3 Bit = cross(Nor, Tan);
    mat3 tbnMat = transpose(mat3(Tan, Bit, Nor));

    v_lightTS = tbnMat * normalize(lightDir);
    v_viewTS = tbnMat * normalize(eyePos - worldPos);

    v_texCoord = texCoord;
    gl_Position = projMat * viewMat * vec4(worldPos, 1.0);
}