#version 450 core

in vec2 uv;
out vec4 fragment_color;

// uniform sampler2D u_texture_1;
// uniform sampler2D u_texture_2;

void main() {
    // fragment_color = mix(texture(u_texture_1, uv), texture(u_texture_2, uv), 0.2);
    fragment_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
};
