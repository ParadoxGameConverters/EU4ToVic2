#pragma once

#ifdef _WIN32
#include "../../../extlibs/lzma/Types.h"
#endif

#ifdef linux
#include "../../../extlibs/lzma/unix/Types.h"
#endif

namespace detail
{
  class lzma_alloc
    : public ISzAlloc
  {
    public:
      lzma_alloc()
      {
        this->Alloc = [](void*, size_t size)   { return malloc(size); };
        this->Free  = [](void*, void* address) { free(address); };
      }
  };
}
