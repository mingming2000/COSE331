#version 300 es

precision mediump float;
uniform vec3 lightDir;

uniform sampler2D colorMap, normalMap;

//in vec3 v_view, v_light, v_normal;
in vec3 v_lightTS, v_viewTS;    // tangent space
in vec2 v_texCoord;

layout(location = 0) out vec4 fragColor;

void main() {
    // normal mapping
    vec3 normal = normalize(2.0 * texture(normalMap, v_texCoord).xyz - 1.0);
    //vec3 light = normalize(lightDir);
    //vec3 view = normalize(v_view);
    vec3 light = normalize(v_lightTS);  // light vec in tan space
    vec3 view = normalize(v_viewTS);    // view vec in tan space

    vec3 lightColor = vec3(1.0);
    float shiness = 30.0;
    vec3 matSpec = vec3(0.05);
    vec3 srcAmbi = vec3(0.0);

    // vec3 normal = normalize(v_normal);
    vec3 color = texture(colorMap, v_texCoord).rgb;

    // diffuse term
    vec3 matDiff = texture(colorMap, v_texCoord).rgb;
    vec3 diff = max(dot(normal, light), 0.0) * lightColor * matDiff;

    // specular term
    vec3 refl = 2.0 * normal * dot(normal, light) - light;
    vec3 spec = pow(max(dot(refl, view), 0.0), shiness) * matSpec;

    vec3 final_color = diff + spec;
//    vec3 final_color = diff;  // for debugging
    fragColor = vec4(final_color, 1.0);
}