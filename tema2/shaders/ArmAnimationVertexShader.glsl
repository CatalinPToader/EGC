#version 330

#define PI 3.1415926

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float timeSinceStop;
uniform bool shouldAnimate;
uniform vec3 p_pos;

// Output
out vec2 texcoord;
out float dist;

float easeInSine(float x)
{
    return 1 - cos((x * PI) / 2);
}

void main()
{
    texcoord = v_texture_coord;
    dist = 0;

    vec3 pos = v_position;

    if (shouldAnimate)
        pos.y += easeInSine(timeSinceStop) / 20;

    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
