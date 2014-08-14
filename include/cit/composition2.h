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

    template<typename F, typename... Fs>
    class _composition : public std::unary_function<
        typename _composition<Fs...>::argument_type,
        typename F::result_type
    > {
        const F & _f;
        const _composition<Fs...> & _fs;

      public:
        _composition(const F & f, const Fs &... fs) : _f(f), _fs(fs...) {};
        inline typename F::result_type operator() (
            const typename _composition<Fs...>::argument_type arg
        ) const {
            return _f(_fs(arg));
        };

    };

    template<>
    template<typename F>
    class _composition<F> : public std::unary_function<
        typename F::argument_type,
        typename F::result_type
    >{
        const F & _f;

      public:
        _composition(const F & f) : _f(f) {};
        inline const typename F::result_type operator() (
            const typename F::argument_type arg
        ) const {
            return _f(arg);
        };

    };

    template<typename F, typename... Fs>
    _composition<F, Fs...> composition(const F & f, const Fs &... fs) {
        return _composition<F, Fs...>(f, fs...);
    };

}

#endif /* _CITERTOOLS_CPP11_ */

#endif /* _CITERTOOLS_COMPOSITION2_H_ */
