#ifndef _CITERTOOLS_IDENTITY_H_
#define _CITERTOOLS_IDENTITY_H_

#include <utility>
#include <functional>

/*
 * So small it doesn't deserve its own file
 * std::identity newer made it into the std version of the STL
 */

namespace cit {
    template<typename T>
    class identity : public std::unary_function<T, T> {
        public:
            inline const T & operator() (const T & value) const {
                return value;
            }
            
#ifdef _CITERTOOLS_CPP11_

            template<typename U>
            inline T && operator() (U && value) const {
                return std::forward<T>(value);
            }
#endif /* N _CITERTOOLS_CPP11_ */

    };

}

#endif /* _CITERTOOLS_IDENTITY_H_ */

