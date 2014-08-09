#ifndef _CITERTOOLS_TUPLE_H_
#define _CITERTOOLS_TUPLE_H_

#include <cit/definition.h>

#include <tuple>

#ifndef _CITERTOOLS_CPP11_
#error "Nothing in <cit/map.h> can be used when not using C++11!"
#else

namespace cit {
    template<size_t N>
    struct tuple_to_args_t {
        template<typename FunctorT, typename... TupleArgs, typename... Args>
        static auto call(FunctorT & functor, std::tuple<TupleArgs...> & tuple, Args &... args)
                ->decltype(tuple_to_args_t<N-1>::call(functor, tuple, std::get<N-1>(tuple), args...)) {
            return tuple_to_args_t<N-1>::call(functor, tuple, std::get<N-1>(tuple), args...);
        }
    };

    template<>
    struct tuple_to_args_t<0> {
        template<typename FunctorT, typename... TupleArgs, typename... Args>
        static auto call(FunctorT & functor, std::tuple<TupleArgs...> & tuple, Args &... args)
                ->decltype(functor(args...)) {
            return functor(args...);
        }
    };

    /*
     * Generic functor, unwraps the tuple and calls a method.
     */

    template<typename FunctorT, typename... TupleArgs, typename... Args>
    inline auto tuple_to_args(FunctorT & functor, std::tuple<TupleArgs...> & tuple)
            ->decltype(tuple_to_args_t<sizeof...(TupleArgs)>::call(functor, tuple)) {
        return tuple_to_args_t<sizeof...(TupleArgs)>::call(functor, tuple);
    }
}

#endif /* _CITERTOOLS_CPP11_ */

#endif /* N _CITERTOOLS_TUPLE_H_ */

