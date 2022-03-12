#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 p_pos;
uniform bool hud;

// Output
out vec2 texcoord;
out float dist;


void main()
{
    texcoord = v_texture_coord;
    vec3 world_position = (Model * vec4(v_position,1)).xyz;

    dist = distance(p_pos, world_position);
    if (hud)
        dist = 0;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
