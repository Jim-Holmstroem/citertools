#ifndef _CITERTOOLS_DEFINITION_H_
#define _CITERTOOLS_DEFINITION_H_

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

#if !defined(_MSC_VER) && !defined(__GNUG__)
#warning "I don't fully understand your compiler, no C++11 enabled"
#endif /* N _MSC_VER || __GNUG__ */

namespace cit {
    
}

#endif /* N _CITERTOOLS_DEFINITION_H_ */

