#include "./frameBuffer.hpp"
#include "util/gl.hpp"

namespace aur {

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, FBFlag flags) {
  GLC(glGenFramebuffers(1, &id_));
  bind();

  if (flags & FB::COLOR) 
    attachments_.insert({ FB::COLOR, createAttachment(width, height, GL_COLOR_ATTACHMENT0, AttType::TEXTURE, GL_RGB) });

  if (flags & FB::DEPTH) 
    attachments_.insert({ FB::DEPTH, createAttachment(width, height, GL_DEPTH_ATTACHMENT, AttType::BUFFER, GL_DEPTH_COMPONENT) });

  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
  unbind();
}

FrameBuffer::~FrameBuffer() {
  GLC(glDeleteFramebuffers(1, &id_));
  for (auto& [_, att] : attachments_) {
    GLC(att.first == AttType::TEXTURE
      ? glDeleteTextures(1, &att.second)
      : glDeleteRenderbuffers(1, &att.second);
    );
  }
  attachments_.clear();
}

void FrameBuffer::bind() {
  GLC(glBindFramebuffer(GL_FRAMEBUFFER, id_));
}

void FrameBuffer::unbind() {
  GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

unsigned int FrameBuffer::getAttachment(FBFlag type) const {
  if (!attachments_.contains(type)) return 0;
  return attachments_.at(type).second;
}

std::pair<FB::AttType, unsigned int> FrameBuffer::createAttachment(
  unsigned int width, 
  unsigned int height, 
  int attachment, 
  AttType type, 
  int format
) const {
  unsigned int id;

  if (type == AttType::TEXTURE) {
    GLC(glGenTextures(1, &id));
    GLC(glBindTexture(GL_TEXTURE_2D, id));
    GLC(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id, 0));
  }
  else {
    GLC(glGenRenderbuffers(1, &id));
    GLC(glBindRenderbuffer(GL_RENDERBUFFER, id));
    GLC(glRenderbufferStorage(GL_RENDERBUFFER, format, width, height));
    GLC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, id));
  }

  return { type, id };
}

}
