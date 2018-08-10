#version 410

in vec4 position;
in vec2 texcoord;

out vec2 vTexCoord;


void main() {
    vTexCoord = texcoord;
    gl_Position = position;
}
