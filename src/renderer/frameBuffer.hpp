#pragma once

#include <cstdint>
#include <map>
#include <utility>

namespace aur {

using FBFlag = std::uint8_t;

class FrameBuffer {
public:
  FrameBuffer(unsigned int width, unsigned int height, FBFlag flags);
  ~FrameBuffer();

  void bind();
  void unbind();

  unsigned int getAttachment(FBFlag type) const;

  static constexpr FBFlag COLOR{ 1 << 0 };
  static constexpr FBFlag DEPTH{ 1 << 1 };

private:
  unsigned int id_;
  enum class AttType { TEXTURE, BUFFER };
  std::map<FBFlag, std::pair<AttType, unsigned int>> attachments_;
  std::pair<AttType, unsigned int> createAttachment(unsigned int width, unsigned int height, int attachment, AttType type, int format) const;
};

using FB = FrameBuffer;

}
