#pragma once

#include "./texture.hpp"
#include <array>

namespace aur {

class Cubemap : public Texture {
public:
  Cubemap(
    const std::string& pathPrefix, 
    const std::array<std::string, 6>& faces = {
      "right.bmp",
      "left.bmp",
      "bottom.bmp",
      "top.bmp",
      "front.bmp",
      "back.bmp"
  });
  void load(int format);

private:
  const std::string pathPrefix_;
  const std::array<std::string, 6> faces_;
};

}
