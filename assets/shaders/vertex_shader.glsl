#version 450 core
layout(location = 0) in vec3 attrib_position;
layout(location = 1) in vec2 attrib_uv;

out vec2 uv;
uniform mat4 u_transform_matrix;

void main() {
    gl_Position = u_transform_matrix * vec4(attrib_position, 1.0);
    uv = attrib_uv;
};
