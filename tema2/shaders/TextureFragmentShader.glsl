#version 330

// Input
in vec2 texcoord;
in float dist;

// Uniform properties
uniform sampler2D texture_1;
uniform bool hasTex;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec4 color = texture2D(texture_1, texcoord);

    if (color.w < 0.5f)
        discard;

    vec4 color2;

    if (hasTex)
        color2 = color;
    else
        color2 = vec4(1);

    //out_color = color2;
    out_color = mix(color2, vec4(0.05), clamp(dist / 40, 0, 1));
}
