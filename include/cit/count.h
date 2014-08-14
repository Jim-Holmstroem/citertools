#ifndef _CITERTOOLS_COUNT_H_
#define _CITERTOOLS_COUNT_H_

#include <cit/definition.h>
#include <numeric>
#include <utility>
#include <tuple>
#include <functional>

#ifndef _CITERTOOLS_CPP11_
#error "Nothing in <cit/map2.h> can be used when not using C++11!"
#else

namespace cit2 {

    class count {
        const size_t & _start;
      public:
        class const_iterator {
            size_t _i;
            bool _undefined;
          public:
            typedef size_t value_type;
            const_iterator(size_t i) : _i(i), _undefined(false) {};
            const_iterator(bool undefined) : _i(0), _undefined(true) {};

            bool operator==(const const_iterator & it) {
                return  ~(_undefined || it._undefined) && _i == it._i;
            };
            const_iterator operator++() {
                _i++;
                return *this;
            };
            value_type & operator*() const { //const?
                return _i;
            };

        };
        count(const size_t start) : _start(start) {};
        const_iterator begin() const {
            return const_iterator(_start);
        };
        const_iterator end() const {
            return const_iterator(false);
        };
    };

};

#endif /* _CITERTOOLS_CPP11_ */

#endif /* _CITERTOOLS_COUNT_H_ */
