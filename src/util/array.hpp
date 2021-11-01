#pragma once

// transpose column-major array to column-minor and vice versa
template <typename T>
void transposeMajor(T* list, unsigned int nMaj, unsigned int nMin) {
  if (nMin == nMaj) {
    for (unsigned int major = 0; major < nMaj - 1; major++) {
      for (unsigned int minor = major + 1; minor < nMin; minor++) {
        T tmp = list[major * nMin + minor];
        list[major * nMin + minor] = list[minor * nMin + major];
        list[minor * nMin + major] = tmp;
      }
    }
  }
  else {
    T copy[nMaj * nMin];
    for (unsigned int i = 0; i < nMaj * nMin; i++) 
      copy[i] = list[i % nMaj * nMin + i / nMaj];
    memcpy(&list[0], &copy[0], nMaj * nMin * sizeof(T));
  }
}
