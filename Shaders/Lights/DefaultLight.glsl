#ifndef DEFAULTLIGHTSHADER_GLSL
#define DEFAULTLIGHTSHADER_GLSL

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight
{
    vec3 direction;
};

struct PointLight
{
    vec3 position;
    Attenuation attenuation;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    Attenuation attenuation;

    float innerAngle;
    float outerAngle;
};

struct Light
{
    int type;
    vec4 color;

    DirectionalLight directional;
    PointLight point;
    SpotLight spot;
};

vec3 directionalLightDirection(Light light) {
    return -light.directional.direction;
}

vec3 pointLightDirection(Light light, vec3 position) {
    return normalize(light.point.position - position);
}

vec3 spotLightDirection(Light light, vec3 position) {
    return normalize(light.spot.position - position);
}

float directionalLightAttenuation(Light light) {
    return 1.0f;
}

float pointLightAttenuation(Light light, vec3 position) {
    float d = length(light.point.position - position);
    float attenuation = light.point.attenuation.constant +
                        light.point.attenuation.linear * d +
                        light.point.attenuation.quadratic * d * d;

    return 1 / attenuation;
}

float spotLightAttenuation(Light light, vec3 position) {
    vec3 dir = normalize(light.spot.direction);
    float d = length(light.spot.direction);
    
    float attenuation = light.spot.attenuation.constant +
                        light.spot.attenuation.linear * d +
                        light.spot.attenuation.quadratic * d * d;
                        
    vec3 l = normalize(light.spot.position - position);
    float cosRealAngle = dot(l, dir);
    float cosSpotOuter = cos(light.spot.innerAngle / 2.0);
    float radialAttenuation = pow(clamp((cosRealAngle - cosSpotOuter) /
                                        (1.0 - cosSpotOuter), 0.0, 1.0), 1.6);
    return radialAttenuation / attenuation;
}

#ifndef POLYGONALLIGHT_GLSL
vec3 getLightDirection(Light light, vec3 position) {
     switch (light.type) {
        case 0: return directionalLightDirection(light);
        case 1: return pointLightDirection(light, position);
        case 2: return spotLightDirection(light, position);
        default: return vec3(0);
    }
}

vec3 lightContributionFrom(Light light, vec3 position) {
    switch (light.type) {
        case 0: return directionalLightAttenuation(light) * light.color.xyz;
        case 1: return pointLightAttenuation(light, position) * light.color.xyz;
        case 2: return spotLightAttenuation(light, position) * light.color.xyz;
        default: return vec3(0.0f);
    }
}
#endif

uniform Light light;

#endif //DEFAULTLIGHTSHADER_GLSL
