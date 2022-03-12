#version 330

// Input
in vec3 color;
in float time;
in float DUR;
in float dist;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    if (time >= DUR) {
        discard;
    }

    out_color = mix(vec4(color,1), vec4(0.05), clamp(dist/60, 0, 1));
}
