#ifndef _CITERTOOLS_PRODUCT_H_
#define _CITERTOOLS_PRODUCT_H_

#include <cit/definition.h>
#include <numeric>
#include <utility>
#include <tuple>
#include <functional>

#ifndef _CITERTOOLS_CPP11_
#error "Nothing in <cit/map2.h> can be used when not using C++11!"
#else

namespace cit2 {

    template<typename T, typename... Ts>
    class _product {  //TODO alot of code is common with _zip
        const T & _t;
        const _product<Ts...> & _ts;
      public:
        class const_iterator {
            typedef typename T::const_iterator inner_const_iterator;
            typedef typename _product<Ts...>::const_iterator outer_const_iterator;
            const inner_const_iterator & _it; //TODO need to copy at construction and have it modifiable for it++ to work as expected.
            const outer_const_iterator & _oit;
            const outer_const_iterator & _oit_begin;
            const outer_const_iterator & _oit_end;

          public:
            typedef typename std::tuple<typename T::value_type, WHUT(_product<Ts...>::value_type)...> value_type;
            const_iterator(const inner_const_iterator & it, const outer_const_iterator & oit) : _it(it), _iot(iot) {};
            bool operator==(const const_iterator & it) {
                return it == _it || it._oit == _oit;
            };
            const_iterator operator++() {
                _oit++;
                if _oit == _oit_end {
                    _oit = _oit_begin; //TODO needs copyconstructor etc
                    _it++;
                } else {
                    _oit++;
                }

                return *this;
            };
            value_type & operator*() const {  //NOTE dereference operator acts as "evaluate"
                return std::tuple_cat(*_it, *_oit);
            };
        };
        _product(const T & t, const Ts &... ts) : _t(t), _ts(ts...) {};
        const_iterator begin() const {
            return const_iterator(_t.begin(), _ts.begin(), _ts.begin(), _ts.end());
        };
        const_iterator end() const {
            return const_iterator(_t.end(), _ts.end(), _ts.begin(), _ts.end());
        };
    };

    template<>
    template<typename T>
    class _product<T> {  //TODO the same as map(tuple<T>, ...)
                         //TODO code duplication _zip<T>
        const T & _t;
      public:
        class const_iterator {
            typedef typename T::const_iterator inner_const_iterator;
            inner_const_iterator _it;
          public:
            typedef typename std::tuple<typename T::value_type> value_type;
            const_iterator(const inner_const_iterator & it) : _it(it) {};

            bool operator==(const const_iterator & it) {
                return it == _it.it;
            };
            const_iterator operator++() {
                _it++;
                return *this;
            };
            value_type & operator*() const {
                return value_type(*_it);
            };

        };
        _product(const T & t) : _t(t) {};
        const_iterator begin() const {
            return const_iterator(_t.begin());
        };
        const_iterator end() const {
            return const_iterator(_t.end());
        };
    };

    template<typename T, typename... Ts>
    _product<T, Ts...> product(const T & t, const Ts &... ts) {
        return _product<T, Ts...>(t, ts...);
    };

};

#endif /* _CITERTOOLS_CPP11_ */

#endif /* _CITERTOOLS_PRODUCT_H_ */
