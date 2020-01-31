#pragma once
#include "../compression_interface.h"

#include "detail/lzma_handle.h"

#ifdef _WIN32
#include "../../extlibs/lzma/LzmaEnc.h"
#endif

#ifdef __unix__
#include "../../extlibs/lzma/unix/LzmaEnc.h"
#endif

struct lzma_encoder_properties
  : compression_encoder_properties_interface
{
  lzma_encoder_properties()
    : IsMultithreaded(true)
    , CompressionLevel(5)
  {

  }

  void normalize() override
  {
    CLzmaEncProps props;
    props.level = CompressionLevel;
    props.numThreads = IsMultithreaded ? 2 : 1;

    LzmaEncProps_Normalize(&props);

    CompressionLevel = props.level;
    IsMultithreaded = props.numThreads != 1;
  }

  void apply(detail::lzma_handle& handle)
  {
    CLzmaEncProps props;
    props.level = CompressionLevel;
    props.numThreads = IsMultithreaded ? 2 : 1;

    LzmaEnc_SetProps(handle.get_native_handle(), &props);
  }
  
  bool IsMultithreaded;
  int  CompressionLevel;
};
