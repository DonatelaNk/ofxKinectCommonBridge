#version 410

uniform sampler2D uTexColor;

in vec2 vTexCoord;
in float vExistence;

out vec4 out_color;

const float drop = 0.9;


void main() {
    if (vExistence < drop) {
        discard;
    }

    vec4 color = texture(uTexColor, vTexCoord);

    out_color = color;
}
