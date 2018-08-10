#version 410

uniform sampler2D uTexWorld;
uniform vec2 uTexelSize;

in vec2 vTexCoord;

out vec4 out_color;

const float radius = 2.0; // the higher the smoother the normals, but problems appear at the borders


vec3 getWorld(vec2 xy) {
    return texture(uTexWorld, xy).xyz;
}


void main() {
    vec3 v[4];
    vec2 x = vTexCoord;
    vec2 dx = radius * uTexelSize.x * vec2(1.0, 0.0);    // deltas
    vec2 dy = radius * uTexelSize.y * vec2(0.0, 1.0);
    v[0] = getWorld( x-dx-dy );                         // south-west
    v[1] = getWorld( x+dx-dy );                         // south-east
    v[2] = getWorld( x-dx+dy );                         // north-west
    v[3] = getWorld( x+dx+dy );                         // north-east

    vec3 r[2];

    r[0] = cross( v[1] - v[0], v[2] - v[0]);            // face1 normal
    r[1] = cross( v[1] - v[3], v[3] - v[2]);            // face2 normal

    vec4 col = vec4(1.0);
    col.rgb = normalize(r[0] + r[1]);

    // test: reverse z coord
    col.z *= -1;

    out_color = col;
}
