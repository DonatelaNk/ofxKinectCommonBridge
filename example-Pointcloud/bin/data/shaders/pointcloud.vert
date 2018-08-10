#version 410

uniform mat4 modelViewProjectionMatrix; // automatically supplied by OF
uniform sampler2D uTexWorld;
uniform sampler2D uTexNormals;
uniform vec2 uTexelSize;
uniform vec2 uTextureSize; // asumes all textures are the same size
uniform float uClippingNear;
uniform float uClippingFar;
uniform float uMaxJump;
uniform int uStepsJump;

// of attributes - automatically supplied by OF
in vec4 position;
in vec2 texcoord;

// outputs
out vec2 vTexCoord;
out float vExistence;

// jump/discontinuity detection
//const float maxjump = 0.2;


// Check wheteher the coordinate should be clipped
// or wheter the vertex is disconnected from its neighbours
bool jumps(vec2 texCoord) {
	vec3 worldCoord = texture(uTexWorld, texCoord).xyz;

	// check clipping
	if (worldCoord.z < uClippingNear || worldCoord.z > uClippingFar) {
		return true;
	}

	// check jump (aka disconnected)
	//int steps = 2;
	float r = length(worldCoord);
	float r2;

	float left   = texCoord.x - uTexelSize.x * uStepsJump;
	float right  = texCoord.x + uTexelSize.x * uStepsJump;
	float bottom = texCoord.y - uTexelSize.y * uStepsJump;
	float top    = texCoord.y + uTexelSize.y * uStepsJump;
	for (float x = left; x <= right; x += uTexelSize.x) {
		for (float y = bottom; y <= top; y += uTexelSize.y) {
			r2 = length( texture(uTexWorld, vec2(x, y)).xyz );
			if (r - r2 > uMaxJump) {
				return true;
			}
		}
	}
	return false;
}


void main() {
	// calculate texcoord using gl_VertexID to avoid artifacts caused by floating point errors
	int width = int(uTextureSize.x);
	vec2 st;
	st.x = gl_VertexID % width;
	st.y = gl_VertexID / float(width);
	st /= uTextureSize;

	// uncomment this to see artifacts when usign the texcoord instead of gl_VertexID
	// st = texcoord;

	vec3 worldPos = texture(uTexWorld, st).xyz;
	vec4 clipPos = modelViewProjectionMatrix * vec4(worldPos, 1.0);

	bool zero = jumps(st);

	clipPos.z = zero ? uClippingFar : clipPos.z;
	clipPos.w = zero ? 0.0 : clipPos.w;

	vExistence = zero ? 0.0 : 1.0;
	vTexCoord = st;
	gl_Position = clipPos;
}
