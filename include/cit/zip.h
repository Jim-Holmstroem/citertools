#ifndef _CITERTOOLS_COMPOSITION2_H_
#define _CITERTOOLS_COMPOSITION2_H_

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
    class _zip {
        const T & _t;
        const _zip<Ts...> & _ts;
      public:
        class const_iterator {
            typedef typename T::const_iterator inner_const_iterator;
            typedef typename _zip<Ts...>::const_iterator outer_const_iterator;
            const inner_const_iterator & _it;
            const outer_const_iterator & _oit;
          public:
            typedef typename std::tuple<typename T::value_type, WHUT(_zip<Ts...>::value_type)...> value_type;
            const_iterator(const inner_const_iterator & it, const outer_const_iterator & oit) : _it(it), _iot(iot) {};
            bool operator==(const const_iterator & it) {
                return it == _it || it._oit == _oit;
            };
            const_iterator operator++() {
                _it++;
                _oit++;
                return *this;
            };
            value_type & operator*() const {  //NOTE dereference operator acts as "evaluate"
                return std::tuple_cat(*_it, *_oit);
            };
        };
        _zip(const T & t, const Ts &... ts) : _t(t), _ts(ts...) {};
        const_iterator begin() const {
            return const_iterator(_t.begin(), _ts.begin());
        };
        const_iterator end() const {
            return const_iterator(_t.end(), _ts.end());
        };
    };

    template<>
    template<typename T>
    class _zip<T> {  //TODO the same as map(tuple<T>, ...)
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
        _zip(const T & t) : _t(t) {};
        const_iterator begin() const {
            return const_iterator(_t.begin());
        };
        const_iterator end() const {
            return const_iterator(_t.end());
        };
    };

    template<typename T, typename... Ts>
    _zip<T, Ts...> zip(const T & t, const Ts &... ts) {
        return _zip<T, Ts...>(t, ts...);
    };

};

#endif /* _CITERTOOLS_CPP11_ */

#endif /* _CITERTOOLS_COMPOSITION2_H_ */
