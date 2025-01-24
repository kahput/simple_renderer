#version 450 core

in vec2 uv;

out vec4 fragment_color;

uniform vec4 u_color;

uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;

void main() {
    fragment_color = mix(texture(u_texture_1, uv), texture(u_texture_2, uv), 0.2) * u_color;
};
