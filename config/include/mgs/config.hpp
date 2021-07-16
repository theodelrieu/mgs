#pragma once

#ifdef _MSC_VER
#  if _MSVC_LANG >= 201402L
#    define MGS_HAS_CPP14
#    if _MSVC_LANG >= 201703L
#      define MGS_HAS_CPP17
#      define MGS_HAS_CPP20
#    endif
#  endif
#else
#  if __cplusplus >= 201709L
#    define MGS_HAS_CPP20
#    define MGS_HAS_CPP17
#    define MGS_HAS_CPP14
#  elif __cplusplus >= 201703L
#    define MGS_HAS_CPP17
#    define MGS_HAS_CPP14
#  elif __cplusplus >= 201402L
#    define MGS_HAS_CPP14
#  endif
#endif
