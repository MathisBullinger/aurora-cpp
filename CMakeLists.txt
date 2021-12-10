cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

project(aurora LANGUAGES CXX)

SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}/src")

find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/app.cpp
  src/window/window.cpp
  src/renderer/renderer.cpp
  src/renderer/shader.cpp
  src/renderer/mesh.cpp
  src/renderer/vertexBuffer.cpp
  src/renderer/indexBuffer.cpp
  src/renderer/scene.cpp
  src/renderer/camera.cpp
  src/renderer/cameraFree.cpp
  src/renderer/cameraFPS.cpp
  src/renderer/projection.cpp
  src/renderer/vertexArray.cpp
  src/renderer/texture.cpp
  src/renderer/cubemap.cpp
  src/renderer/material.cpp
  src/renderer/frameBuffer.cpp
  src/loaders/texture.cpp
  src/loaders/mesh.cpp
  src/loaders/objParser.cpp
  src/loaders/mtlParser.cpp
  src/input/keyboard.cpp
  src/input/mouse.cpp
  src/input/inputReceiver.cpp
  src/input/cameraController.cpp
  src/util/path.cpp
  src/util/string.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE 
  SDL2::SDL2main 
  SDL2::SDL2-static
  GLEW::GLEW
)
