#ifndef _CITERTOOLS_MAP_H_
#define _CITERTOOLS_MAP_H_

#include <cit/definition.h>
#include <functional>
#include <iterator>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <utility>

#ifndef _CITERTOOLS_CPP11_
#error "Nothing in <cit/map.h> can be used when not using C++11!"
#else

#include <initializer_list>

namespace cit {
    /*
     * These are help functions for map and it's variadic template
     * mumbo-jumbo, don't use!
     */
    
    namespace _map {
        template<typename... Args> inline void pass(Args&...) {}

        /*
         * Helper method that references arguments before calling the bound functor.
         */

        template<typename FunctorT>        
        struct reference_then_call_t {
            FunctorT & functor;

            inline reference_then_call_t(FunctorT & functor) : functor(functor) {}

            template<typename... ArgsT>
            inline auto operator()(const ArgsT  &... args)->decltype(functor(*args...)) {
                return functor(*args...);
            }
        };

        template<size_t N>
        struct for_each_tuple_t {
            template<typename FunctorT, typename... TupleArgs, typename... Args>
            static auto call(FunctorT & functor, std::tuple<TupleArgs...> & tuple, Args &... args)
                    ->decltype(for_each_tuple_t<N-1>::call(functor, tuple, std::get<N-1>(tuple), args...)) {
                return for_each_tuple_t<N-1>::call(functor, tuple, std::get<N-1>(tuple), args...);
            }
        };

        template<>
        struct for_each_tuple_t<0> {
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
        inline auto for_each_tuple(FunctorT & functor, std::tuple<TupleArgs...> & tuple)
                ->decltype(for_each_tuple_t<sizeof...(TupleArgs)>::call(functor, tuple)) {
            return for_each_tuple_t<sizeof...(TupleArgs)>::call(functor, tuple);
        }

        struct increase_t {
            template<typename... Args>
            void operator() (Args &... args) const {
                pass(++args...);
            }
        };

        struct decrease_t {
            template<typename... Args>
            void operator() (Args &... args) const {
                pass(--args...);
            }
        };

        /*
         * iterator: maps multiple iterators to one via a functor.
         *
         */

        template<typename FunctorT, typename EndIteratorT, typename... IteratorsT>
        class iterator {
            private:
                std::tuple<IteratorsT...> iterators;
                EndIteratorT end_iterator;

                _map::reference_then_call_t<FunctorT> reference_then_call;
                
                /*
                 * Stores the current cached FunctorT::result_type
                 */
                
                bool cache_valid; /* True if cache is valid */
                typedef decltype(_map::for_each_tuple(reference_then_call, iterators)) value_type;
                value_type cache;

            public:
                iterator(
                    _map::reference_then_call_t<FunctorT> && reference_then_call,
                    EndIteratorT && end_iterator,
                    std::tuple<IteratorsT...> && iterators)
                        : iterators(std::forward<std::tuple<IteratorsT...>>(iterators)),
                        end_iterator(std::forward<EndIteratorT>(end_iterator)),
                        reference_then_call(std::forward<_map::reference_then_call_t<FunctorT>>(reference_then_call)),
                        cache_valid(false) {}

                /* Only for specification, if you're comparing with end(), do it
                 * using the same container handled to it in the #ctor */
                
                inline bool operator== (const iterator & it) const {
                    return std::get<0>(iterators) == std::get<0>(it.iterators);
                }
                
                inline bool operator!= (const iterator & it) const {
                    return std::get<0>(iterators) != std::get<0>(it.iterators);
                }

                /*
                 * For use with C++11 foreach-style loops that need
                 * begin() and end().
                 */

                inline iterator<FunctorT, EndIteratorT, IteratorsT...>
                        begin() const {
                    return *this;
                }
                
                inline iterator<FunctorT, EndIteratorT, IteratorsT...> end() const {
                    std::tuple<IteratorsT...> new_iterators(iterators);
                    std::get<0>(new_iterators) = end_iterator;
                    return iterator<FunctorT, EndIteratorT, IteratorsT...>(
                        _map::reference_then_call_t<FunctorT>(reference_then_call),
                        EndIteratorT(end_iterator),
                        std::move(new_iterators));
                }

                iterator<FunctorT, EndIteratorT, IteratorsT...> & operator++() {
                    increase_t increase;
                    for_each_tuple(increase, iterators);

                    cache_valid = false;
                    
                    return *this;
                }
                
                iterator<FunctorT, EndIteratorT, IteratorsT...> operator++(int i) {
                    iterator<FunctorT, EndIteratorT, IteratorsT...> it(*this);
                    
                    this->operator++();
                    
                    return it;
                }
                
                inline iterator<FunctorT, EndIteratorT, IteratorsT...> &
                        operator--() {
                    decrease_t decrease;
                    for_each_tuple(decrease, iterators);
                    cache_valid = false;
                    
                    return *this;
                }
                
                iterator<FunctorT, EndIteratorT, IteratorsT...> operator--(int i) {
                    iterator<FunctorT, EndIteratorT, IteratorsT...> it(*this);
                    
                    this->operator--();
                    
                    return it;
                }

                value_type & operator*() {
                    if (!cache_valid) {
                        cache = _map::for_each_tuple(reference_then_call, iterators);
                        cache_valid = true;
                    }
                    
                    return cache;
                }
                
                inline value_type *operator->() {
                    return &this->operator*();
                }
        };
        
        /*
         * Helper method for deducing types.
         */
        template<typename FunctorT, typename EndIteratorT, typename... IteratorsT>
        iterator<FunctorT, EndIteratorT, IteratorsT...> make_iterator(
                FunctorT & functor,
                EndIteratorT && end_iterator,
                std::tuple<IteratorsT...> && iterators) {
            return iterator<FunctorT, EndIteratorT, IteratorsT...>(
                _map::reference_then_call_t<FunctorT>(functor),
                std::forward<EndIteratorT>(end_iterator),
                std::forward<std::tuple<IteratorsT...>>(iterators));
        }
    }
    
    namespace iterator {
        /*
         * something like:
         * for(auto element : cit::iterator::map([] (T1 x, T2 y, T3 z) { return T4(...); }, container.begin(), container.end(), other_container.begin(), another_container.begin()))
         * where element is gotten from return T4
         */

        template<typename FunctorT, typename IteratorT, typename... IteratorsT>
        auto map(
                const FunctorT & functor, const IteratorT & begin,
                const IteratorT & end, const IteratorsT &... iterators)->decltype(_map::make_iterator(functor, end, std::make_tuple(begin, iterators...))) {
            return _map::make_iterator(functor, end, std::make_tuple(begin, iterators...));
        }
    }

    namespace container {
        /*
         * something like:
         * for(auto element : cit::container::map([] (T1 x, T2 y, T3 z) { return T4(...); }, container, other_container, another_container))
         * where element is gotten from return T4
         */

        template<typename FunctorT, typename ContainerT, typename... ContainersT>
        auto map(const FunctorT & functor, const ContainerT & container, const ContainersT &... containers)
                ->decltype(iterator::map(functor, std::begin(container), std::end(container), std::begin(containers)...)) {
            return iterator::map(functor, std::begin(container), std::end(container), std::begin(containers)...);
        }
    }
    
    /*
     * filter: If the functor returns something that decuces to true,
     * the element is added to the output
     */
    
    template<typename FunctorT, typename OutputIteratorT, typename IteratorT>
    void filter(const FunctorT & functor, IteratorT it, const IteratorT & end,
            OutputIteratorT output) {

        for(; it != end; ++it) {
            decltype(*it) ref(*it); // decltype should give a reference.

            if (functor(ref)) {
                output = ref;
                ++output;
            }
        }
    }
    
    template<typename FunctorT, typename OutputIteratorT, typename ContainerT>
    inline void filter(const FunctorT & functor, const ContainerT & container,
            OutputIteratorT output) {
        return filter(functor, container.begin(), container.end(), output);
    }
    
    /*
     * reduce: FunctorT must be a binary_function that add two items
     * should be lambda compatible
     */
    
    template<typename IteratorT, typename FunctorT>
    auto reduce(const FunctorT & func, IteratorT it,
            const IteratorT & end)
                ->typename std::remove_reference<decltype(*it)>::type {

        typedef typename std::remove_const<
            typename std::remove_reference<decltype(*it)>::type>::type
                return_type_t;

        if (it == end)
            return return_type_t();

        return_type_t v(*it);

        for(++it; it != end; ++it)
            v = func(v, *it);

        return v;
    }
    
    /*
     * Alias for mapping containers and initializer_lists
     */
    
    template<typename ContainerT, typename FunctorT>
    inline auto reduce(const FunctorT & func, const ContainerT & container)
            ->decltype(*container.begin()) {
        return reduce(func, container.begin(), container.end());
    }
    
    /*
     * initializer_list special
     */
    
    template<typename T, typename FunctorT>
    T reduce(const FunctorT & func, const std::initializer_list<T> & list) {
        return reduce(func, list.begin(), list.end());
    }
}

#endif /* _CITERTOOLS_CPP11_ */

#endif /* _CITERTOOLS_MAP_H_ */

