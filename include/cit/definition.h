#ifndef _CITERTOOLS_DEFINITION_H_
#define _CITERTOOLS_DEFINITION_H_

#include <cstdio>

/*
 * Global definitions and macros to be used throughout the code base
 */

/*
 * Figure out whether we're using C++11
 * (g++ defines experimental or C++11, for Visual C++ we require >= 2010)
 */

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L || _MSC_VER >= 1600
#define _CITERTOOLS_CPP11_
#endif /* C++11 */

#ifdef _MSC_VER

#define _CITERTOOLS_PRETTY_FUNCTION_ __FUNCDNAME__

#elif defined(__GNUG__)

#define _CITERTOOLS_PRETTY_FUNCTION_ __PRETTY_FUNCTION__

#else
#warning "I don't fully understand your compiler, no C++11 enabled"
#endif /* N _MSC_VER || __GNUG__ */

namespace cit {
    
}

/*
 * Some print debugging
 */

#define _CITERTOOLS_PRINTF(stream, class, ...)		{ fprintf(stream, "%s: line: %u in: %s: %s: ", __FILE__, __LINE__,  \
													  _CITERTOOLS_PRETTY_FUNCTION_, class);                             \
													  fprintf(stream, __VA_ARGS__);                                     \
													  fprintf(stream, "\n"); }

#define DEB_PRINTF(...)  _CITERTOOLS_PRINTF(stdout, "DEBUG", __VA_ARGS__)
#define WARN_PRINTF(...)  _CITERTOOLS_PRINTF(stderr, "WARNING", __VA_ARGS__)
#define ERR_PRINTF(...)  _CITERTOOLS_PRINTF(stderr, "ERROR", __VA_ARGS__)

#endif /* N _CITERTOOLS_DEFINITION_H_ */

