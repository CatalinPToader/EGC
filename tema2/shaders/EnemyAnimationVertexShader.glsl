#version 330

#define PI 3.1415926
#define ANIM_DUR 5
#define HP_MAX 3.0

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform sampler2D texture_1;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float animationTime;
uniform float playerDist;
uniform float mixVal;

// Output
out vec3 color;
out float time;
out float DUR;
out float dist;



void main()
{
    vec3 red1 = vec3(1, 0, 0);
    vec3 red2 = vec3(0.81, 0, 0.2);
    vec3 green1 = vec3(0.7, 1, 0);
    vec3 green2 = vec3(0, 0.81, 0.6);

    float animTime = clamp(animationTime, 0, ANIM_DUR);

    float scale = animTime > 0 ? clamp(10 / playerDist, 0.2, 1) : 0.2;
    float noiseOffset = min(animTime * animTime, -animTime + ANIM_DUR * 2);

    float shrinkval = animTime > ANIM_DUR - 2 ? animTime - (ANIM_DUR - 1.9) : 0;

    vec4 noiseTex = texture2D(texture_1, v_texture_coord * ((sin(animTime) + 1) / 2));

    float scale2 = animTime > 0 ? 1 : 0;

    float sinR = sin(noiseTex.r);
    float sinG = sin(noiseTex.g);
    float sinB = sin(noiseTex.b);

    vec3 pos = v_position + v_normal * (sinR * sinG * sinB) * noiseOffset * scale *  scale2 - v_position * shrinkval;

    vec3 color1 = mix(red1, green1, mixVal);
    vec3 color2 = mix(red2, green2, mixVal);

    float sinX = sin(pos.x * pos.z * PI);
    float sinY = sin(pos.y * pos.z * PI);
    float mixValue = clamp((sinX + sinY + 1) * 0.33, 0, 1);
    color = mix(color1,color2,mixValue);

    time = animTime;

    DUR = ANIM_DUR;

    dist = playerDist;

    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
