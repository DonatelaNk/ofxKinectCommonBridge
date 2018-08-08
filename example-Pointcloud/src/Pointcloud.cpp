#include "Pointcloud.h"

using namespace glm;


Pointcloud::Pointcloud(ofxKinectCommonBridge& k) : kinect(k) {
	parameters.setName("pointcloud");
	parameters.add(clippingNear.set("clipping near", 0.5f, 0.0f, 10.0f));
	parameters.add(clippingFar.set("clipping far", 3.0f, 0.0f, 10.0f));
	parameters.add(jumpMax.set("jump max", 0.2f, 0.1f, 1.0f));
	parameters.add(jumpStep.set("jump step", 2, 1, 10));
}


void Pointcloud::setup(int _width, int _height, int _steps) {
	shaderNormals.load("shaders/passthrough.vert", "shaders/normals.frag");
	shaderPointcloud.load("shaders/Pointcloud.vert", "shaders/Pointcloud.frag");

	width = _width;
	height = _height;
	steps = _steps;

	fboNormals.allocate(width, height, GL_RGB32F);
	fboNormals.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	createMesh();
}



void Pointcloud::createMesh() {
	int w = floor(width / steps) + (((int)width % steps == 0) ? 0 : 1);
	int h = floor(height / steps) + (((int)height % steps == 0) ? 0 : 1);

	mesh.clear();
	mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_TRIANGLES);

	// add vertices
	for (int y = 0; y < h; y += 1) {
		for (int x = 0; x < w; x += 1) {
			// we start with an arbitrary position 
			// the final vertex position will be calculated in the shader
			mesh.addVertex(vec3(x * steps, y * steps, 0));

			// normalized texture coords
			float s = float(x * steps) / float(width);
			float t = float(y * steps) / float(height);
			mesh.addTexCoord(vec2(s, t));
		}
	}
	// stich vertices
	for (int y = 0; y < h - 1; y += 1) {
		for (int x = 0; x < w - 1; x += 1) {
			int bottomLeft = w * y + x;
			int bottomRight = w * y + (x + 1);
			int topLeft = w * (y + 1) + x;
			int topRight = w * (y + 1) + (x + 1);

			// upper left triangle
			const ofIndexType indicesTopLeft[3] = { topLeft, bottomLeft, bottomRight };
			mesh.addIndices(indicesTopLeft, 3);

			// bottom right triangle
			const ofIndexType indicesBottomRight[3] = { topLeft, bottomRight, topRight };
			mesh.addIndices(indicesBottomRight, 3);
		}
	}

}


void Pointcloud::createFullScreenQuad() {
	// -1.0 to +1.0 is the full viewport (screen) if you use these as vertices in your shader
	// (without multiplying by model, view and projection matrices)
	vec3 vertices[4] = {
		vec3(1.0, 1.0, 0.0),
		vec3(-1.0, 1.0, 0.0),
		vec3(1.0, -1.0, 0.0),
		vec3(-1.0, -1.0, 0.0)
	};

	// the 6 indices representing the two triangles making up our quad
	ofIndexType indices[6] = {
		0, 1, 2,
		2, 1, 3
	};

	// Texture coordinates  very from 0.0 to 1.0 when they are in normalized format
	// ofDisableArbTex() set that we're using normalized texture coordinates
	vec2 texCoords[4] = {
		vec2(1.0, 1.0),
		vec2(0.0, 1.0),
		vec2(1.0, 0.0),
		vec2(0.0, 0.0)
	};

	quad.addVertices(vertices, 4);
	quad.addTexCoords(texCoords, 4);
	quad.addIndices(indices, 6);
}


void Pointcloud::update() {
	if (kinect.isFrameNewDepth()) {
		fboNormals.begin();
		{
			ofClear(0);

			shaderNormals.begin();
			shaderNormals.setUniformTexture("uTexWorld", kinect.getWorldTexture(), 0);
			shaderNormals.setUniform2f("uTexelSize", vec2(1.0f / width, 1.0f / height));
			{
				quad.draw();
			}
			shaderNormals.end();
		}
		fboNormals.end();
	}
}


void Pointcloud::draw() {
	shaderPointcloud.begin();
	shaderPointcloud.setUniformTexture("uTexWorld", kinect.getWorldTexture(), 0);
	shaderPointcloud.setUniformTexture("uTexNormals", fboNormals.getTexture(), 1);
	shaderPointcloud.setUniformTexture("uTexColor", kinect.getColorTexture(), 2);
	shaderPointcloud.setUniform2f("uTexelSize", vec2(1.0f / width, 1.0 / height));
	shaderPointcloud.setUniform2f("uTextureSize", vec2(width, height));
	shaderPointcloud.setUniform1f("uClippingNear",clippingNear); // kinect uses metres
	shaderPointcloud.setUniform1f("uClippingFar", clippingFar);  //
	shaderPointcloud.setUniform1f("uMaxJump", jumpMax);
	shaderPointcloud.setUniform1i("uStepsJump", jumpStep);
	{
		mesh.draw(ofPolyRenderMode::OF_MESH_FILL);
	}
	shaderPointcloud.end();
}


