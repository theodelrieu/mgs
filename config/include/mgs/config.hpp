#pragma once

#ifdef _MSC_VER
#  if _MSVC_LANG >= 201402L
#    define MGS_HAS_CPP14 1
#    if _MSVC_LANG >= 201703L
#      define MGS_HAS_CPP17 1
#      if _MSVC_LANG > 201703L
#        define MGS_HAS_CPP20 1
#      endif
#    endif
#  endif
#else
#  if __cplusplus >= 201402L
#    define MGS_HAS_CPP14 1
#    if __cplusplus >= 201703L
#      define MGS_HAS_CPP17 1
#      if __cplusplus > 201703L
#        define MGS_HAS_CPP20 1
#      endif
#    endif
#  endif
#endif
