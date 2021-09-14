#pragma once

#ifdef _MSC_VER
#  if _MSVC_LANG > 201703L
#    define MGS_HAS_CPP20
#    define MGS_HAS_CPP17
#  elif _MSVC_LANG > 201402L
#    define MGS_HAS_CPP17
#  endif
#else
#  if __cplusplus >= 201709L
#    define MGS_HAS_CPP20
#    define MGS_HAS_CPP17
#  elif __cplusplus >= 201703L
#    define MGS_HAS_CPP17
#  endif
#endif
