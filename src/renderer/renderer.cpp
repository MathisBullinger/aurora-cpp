#include "renderer.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "./viewUtil.hpp"
#include "util/gl.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>

#include "loaders/texture.hpp"
#include "loaders/mesh.hpp"

namespace aur {

Renderer::Renderer() {
	GLC(glEnable(GL_DEPTH_TEST));
	GLC(glDepthFunc(GL_LESS));

	shader = new Shader("../resources/shaders/basic.vert", "../resources/shaders/basic.frag");
  shader->use();

	unsigned int vao;
  GLC(glGenVertexArrays(1, &vao));
  GLC(glBindVertexArray(vao));

	auto obj = new Mesh("../resources/meshes/box.obj");
	objects[obj] = std::vector<Object*>{};
	objects[obj].push_back(new Object(*obj));

  mvpID = glGetUniformLocation(shader->program, "MVP");

	// loader::Texture textureLoader;
	// auto texId = textureLoader.loadBMP("../resources/textures/foo.bmp");
	// glBindTexture(GL_TEXTURE_2D, texId);
}

Renderer::~Renderer() {
	for (auto [mesh, objs] : objects) {
	  delete mesh;
		for (auto obj : objs) delete obj;
	}
  delete shader;
};

void Renderer::render() {
	auto view = aur::lookAt({0, 4, -8}, {0, 1.5, 0}, {0, 1, 0});
  auto projection = aur::perspective(800.f/600, M_PI / 4, 0.1f, 1000.f);

	auto rotAx = vec3<float>{0,1,0}.normal();
	
	for (auto [mesh, objs] : objects) {
		// TODO: bind mesh

		for (auto obj : objs) {
			obj->rotation = Quaternion{rotAx, 0.01} * obj->rotation;
			
			auto model = matrix::translation(obj->translation) * obj->rotation.matrix() * matrix::scale(obj->scale);
  		auto MVP = projection * view * model;
			glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP.values[0]);

			// GLC(glDrawArrays(GL_TRIANGLES, 0, obj->mesh.numVertices()));
			GLC(glDrawElements(GL_TRIANGLES, obj->mesh.countIndices(), GL_UNSIGNED_INT, (void*)0));
		}
	}
	

}

void Renderer::setWireMode(bool on) {
  GLC(glPolygonMode(GL_FRONT_AND_BACK, on ? GL_LINE : GL_FILL));
}

}
