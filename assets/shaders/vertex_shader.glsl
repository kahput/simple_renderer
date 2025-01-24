#version 450 core
layout(location = 0) in vec3 attrib_position;

void main() {
    gl_Position = vec4(attrib_position.x, -attrib_position.y, attrib_position.z, 1.0);
};
