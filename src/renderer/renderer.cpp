#include "renderer.hpp"

Renderer::Renderer() {
  float positions[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f,
  };
  
  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  unsigned int vao;
  GLC(glGenVertexArrays(1, &vao));
  GLC(glBindVertexArray(vao));
  
  unsigned int buffer;
  GLC(glGenBuffers(1, &buffer));
  GLC(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  GLC(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

  GLC(glEnableVertexAttribArray(0));
  GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

  unsigned int ibo;
  GLC(glGenBuffers(1, &ibo));
  GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

  shader = new Shader("../resources/shaders/basic.vert", "../resources/shaders/basic.frag");
  shader->use();

  int uni_loc = glGetUniformLocation(shader->program, "u_Color");
  ASSERT(uni_loc != -1);
  glUniform4f(uni_loc, 0.0f, 1.0f, 0.0f, 1.0f);
}

Renderer::~Renderer() {
  delete shader;
};

void Renderer::render() {
  GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

void Renderer::setWireMode(bool on) {
  GLC(glPolygonMode(GL_FRONT_AND_BACK, on ? GL_LINE : GL_FILL));
}

