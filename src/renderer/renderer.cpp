#include "renderer.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "./viewUtil.hpp"
#include "util/gl.hpp"
#include <iostream>
#include <cmath>
#include <vector>

#include "loaders/texture.hpp"
#include "loaders/mesh.hpp"

namespace aur {

Renderer::Renderer() {
	std::vector<float> vertices;
	std::vector<float> uvs;
	std::vector<float> normals;

	loader::Mesh meshLoader;
	meshLoader.loadOBJ("../resources/meshes/box.obj", vertices, uvs, normals);
  
  unsigned int vao;
  GLC(glGenVertexArrays(1, &vao));
  GLC(glBindVertexArray(vao));

  unsigned int vertexbuffer;
  GLC(glGenBuffers(1, &vertexbuffer));
  GLC(glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer));
  GLC(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW));

  unsigned int colorbuffer;
	GLC(glGenBuffers(1, &colorbuffer));
	GLC(glBindBuffer(GL_ARRAY_BUFFER, colorbuffer));
	GLC(glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), &uvs[0], GL_STATIC_DRAW));

  GLC(glEnable(GL_DEPTH_TEST));
	GLC(glDepthFunc(GL_LESS));

  shader = new Shader("../resources/shaders/basic.vert", "../resources/shaders/basic.frag");
  shader->use();

  // 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

  // view stuff


  mvpID = glGetUniformLocation(shader->program, "MVP");

	loader::Texture textureLoader;
	auto texId = textureLoader.loadBMP("../resources/textures/foo.bmp");
	glBindTexture(GL_TEXTURE_2D, texId);
}

Renderer::~Renderer() {
  delete shader;
};

void Renderer::render() {
	incr++;
	float dist = 5;
	float x = sin(incr / 100.f) * dist;
	float z = cos(incr / 100.f) * dist;
	float y = sin(incr / 133.f) * (dist / 2);
	
	auto view = aur::lookAt({x, y, z}, {0, 0, 0}, {0, 1, 0});
  auto projection = aur::perspective(800.f/600, M_PI / 4, 0.1f, 100.f);
  auto MVP = projection * view;
  glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP.values[0]);
	
  GLC(glDrawArrays(GL_TRIANGLES, 0, 12*3));
}

void Renderer::setWireMode(bool on) {
  GLC(glPolygonMode(GL_FRONT_AND_BACK, on ? GL_LINE : GL_FILL));
}

}
