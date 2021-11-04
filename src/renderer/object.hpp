#include "renderer/mesh.hpp"
#include "math/quaternion.hpp"

namespace aur {

class Object {
public:
  Object(const Mesh& mesh);

  vec3<float> translation {0, 0, 0};
  vec3<float> scale {1, 1, 1};
  Quaternion rotation;

  const Mesh& mesh;
};
  
}
