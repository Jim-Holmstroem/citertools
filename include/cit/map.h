#ifndef _CITERTOOLS_MAP_H_
#define _CITERTOOLS_MAP_H_

#include <cit/definition.h>
#include <numeric>
#include <utility>
#include <tuple>

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
        template<typename IteratorT>
        inline IteratorT & increase(IteratorT & it) {
            return ++it;
        }
        
        template<typename IteratorT>
        inline typename IteratorT::value_type & reference(IteratorT & it) {
            return *it;
        }
        
        /*
         * Pass does nothing but makes the compiler expand the Args and thus
         * call a function on every element in it, see below
         */
        
        template<typename... Args> inline void pass(Args&...) {
            
        }
        
        /*
         * Obtain the begin iterator from a container
         */
        
        template<typename ContainerT>
        typename ContainerT::const_iterator container(const ContainerT & container) {
            return container.begin();
        }
        
        /*
         * Helper function for ++'ing all elements in a tuple full of iterators
         */
        
        template<typename TupleT, std::size_t N>
        struct _increase_tuple {
            static inline TupleT & increase(TupleT & tuple) {
                ++std::get<N-1>(tuple);
                return _increase_tuple<TupleT, N-1>(tuple);
            }
            
            static inline TupleT & decrease(TupleT & tuple) {
                --std::get<N-1>(tuple);
                return _increase_tuple<TupleT, N-1>(tuple);
            }
        };
        
        template<typename TupleT>
        struct _increase_tuple<TupleT, 1> {
            static inline TupleT & increase(TupleT & tuple) {
                ++std::get<0>(tuple);
                
                return tuple;
            }
            
            static inline TupleT & decrease(TupleT & tuple) {
                --std::get<0>(tuple);
                
                return tuple;
            }
        };
        
        template<typename... Args>
        inline std::tuple<Args...> & increase_tuple(std::tuple<Args...> & tuple) {
            return _increase_tuple<decltype(tuple), sizeof...(Args)>::increase(tuple);
        }
        
        template<typename... Args>
        inline std::tuple<Args...> & decrease_tuple(std::tuple<Args...> & tuple) {
            return _increase_tuple<decltype(tuple), sizeof...(Args)>::decrease(tuple);
        }
        
        /*
         * Now, we need a functor for unpacking the tuple and calling the functor
         */
        
        template<size_t N>
        struct unpack_tuple_functor {
            // FIXME: Mod Args to Args& and in the tuple too
            template<typename FunctorT, typename IteratorT, typename... TupleArgs, typename... Args>
            static typename FunctorT::result_type call(const FunctorT & functor, IteratorT & it, std::tuple<TupleArgs...> & tuple, Args... args) {
                return unpack_tuple_functor<N-1>::call(functor, it, tuple, std::get<N-1>(tuple), args...);
            }
        };
        
        template<>
        struct unpack_tuple_functor<0> {
            // FIXME: Mod Args to Args& and in the tuple too
            template<typename FunctorT, typename IteratorT, typename... TupleArgs, typename... Args>
            static typename FunctorT::result_type call(const FunctorT & functor, IteratorT & it, std::tuple<TupleArgs...> & tuple, Args... args) {
                return functor(it, args...);
            }
        };
        
        // FIXME: Eliminate return_type by having it as an argument instead of return
        
        template<typename FunctorT, typename IteratorT, typename... TupleArgs, typename... Args>
        typename FunctorT::result_type call_unpack_tuple(const FunctorT & functor, IteratorT & it, std::tuple<TupleArgs...> & tuple) {
            return unpack_tuple_functor<sizeof... (Args)>::call(functor, it, tuple);
        }
    }
    
    /*
     * map iterator: maps multiple iterators to one via a functor
     *
     * *DONT* use it, use the help function map() below
     *
     */
    
    template<typename FunctorT, typename IteratorT, typename... Args>
    class map_iterator {
        public:
            map_iterator(const FunctorT & functor, const IteratorT & it, const IteratorT & end, const Args&... its) : m_functor(functor), m_it(it), m_end(end), m_its(its...), m_cache_valid(false) { }
            
            map_iterator(const map_iterator & it) : m_functor(it.m_functor), m_it(it.m_it), m_its(it.m_its), m_cache_valid(false) { }
            
            /* Only for specification, if you're comparing with end(), do it
             * using the same container handled to it in the #ctor */
            
            bool operator== (const map_iterator & it) const {
                return m_it == it.m_it;
            }
            
            bool operator!= (const map_iterator & it) const {
                return m_it != it.m_it;
            }
            
            /*
             * for use with the IteratorT::end()
             */
            
            bool operator==(const IteratorT & it) const {
                return m_it == it;
            }
            
            bool operator!=(const IteratorT & it) const {
                return m_it != it;
            }
            
            /*
             * For use with C++11 foreach-style loops that need end()
             * a bit of a hack
             */
            
            // FIXME: Can we return by reference?
            
            inline map_iterator<FunctorT, IteratorT, Args...> begin() const {
                return map_iterator<FunctorT, IteratorT, Args...>(*this);
            }
            
            inline const IteratorT & end() const {
                return m_end;
            }
            
            inline map_iterator<FunctorT, IteratorT, Args...> & operator++() {
                ++m_it;
                _map::increase_tuple(m_its);
                m_cache_valid = false;
                
                return *this;
            }
            
            map_iterator<FunctorT, IteratorT, Args...> operator++(int i) {
                map_iterator<FunctorT, IteratorT, Args...> it(*this);
                
                this->operator++();
                
                return it;
            }
            
            inline map_iterator<FunctorT, IteratorT, Args...> & operator--() {
                --m_it;
                _map::decrease_tuple(m_its);
                m_cache_valid = false;
                
                return *this;
            }
            
            map_iterator<FunctorT, IteratorT, Args...> operator--(int i) {
                map_iterator<FunctorT, IteratorT, Args...> it(*this);
                
                this->operator--();
                
                return it;
            }
            
            // FIXME: Cache the functor, only call it when required
            // thus we'll be able to implement operator-> too (it's a pointer)
            // FIXME: Return value should be auto to avoid requiring result_type
            
            // FIXME: One way to deduce return type might be having it as a template
            // argument, where the template argument is deduced in cit::map
            // below by doing decltype on the return type of the given FunctorT
            // right there
            
            typename FunctorT::result_type & operator*() {
                if (!m_cache_valid) {
                    m_cache = _map::call_unpack_tuple(m_functor, m_it, m_its);
                    m_cache_valid = true;
                }
                
                return m_cache;
            }
            
            auto operator->() -> decltype(&this->operator*()) {
                return &this->operator*();
            }
            
        private:
            const FunctorT & m_functor;
            IteratorT m_it;
            const IteratorT & m_end;
            std::tuple<Args...> m_its;
            
            /*
             * Stores the current cached FunctorT::result_type
             */
            
            bool m_cache_valid; /* True if cache is valid */
            typename FunctorT::result_type m_cache;
    };
    
    /*
     * Here's the funny piece of code
     *
     * something like:
     * for(auto element : map([] (T1 x, T2 y, T3 z) { return T4(...); }, container.begin(), container.end(), other_container.begin(), another_container.begin()))
     * where element is gotten from return T4
     */
    
    template<typename FunctorT, typename IteratorT, typename... Args>
    map_iterator<FunctorT, IteratorT, Args...> map(const FunctorT & functor, const IteratorT & it, const IteratorT & end, const Args&... args) {
        return map_iterator<FunctorT, IteratorT, Args...>(functor, it, end, args...);
    }

    /*
     * map functionality; iterate over all the iterators in parallel and
     * call output++ = functor(it1, it2, ..., itN)
     *
     * Note that in difference to Python, we can't assume the return type
     * thus an OutputIteratorT is given, could be a back_inserter, vector,
     * array, pointer, ostream_iterator etc.
     *
     *
     * FunctorT can take as *many* arguments as you want,
     * DON'T need to implement any function interface!
     * additional iterators *ONLY* need begin, *NO* end is required!!!
     *
     * example usage: map(std::plus<int>(),
     *                    std::ostream_iterator<int>(std::cout, " "),
     *                    some_int_container.begin(),
     *                    some_int_container.end(),
     *                    some_other_int_container.begin());
     *
     * will print the sum of two ranges out on the std::cout
     * there aren't that many STL functors that take more than two arguments
     * but in the test suite, a variadic template sum functor is given
     */
    
    template<typename FunctorT, typename OutputIteratorT, typename IteratorT, typename... Args>
    void map(const FunctorT & functor, OutputIteratorT output, IteratorT it, const IteratorT & end, Args... its) {
        for(; it != end; ++it) {
            output = functor( _map::reference(it), _map::reference(its)... );
            _map::pass ( _map::increase(its)...);
            ++output;
        }
    }
    
    /*
     * Alias for containers, might be more convenient in some cases
     */
    
    /*template<typename FunctorT, typename OutputIteratorT, typename ContainerT, typename... Args>
    inline void mapc(const FunctorT & functor, OutputIteratorT output, const ContainerT & container, const Args &... its) {
        return map(functor, output, container.begin(), container.end(), _map::container(its)... );
    }*/
    
    /*
     * filter: If the functor returns something that decuces to true,
     * the element is added to the output
     */
    
    template<typename FunctorT, typename OutputIteratorT, typename IteratorT>
    void filter(const FunctorT & functor, IteratorT it, const IteratorT & end, OutputIteratorT output) {
        for(; it != end; ++it) {
            typename IteratorT::value_type & ref(*it);
            
            if (functor(ref)) {
                output = ref;
                ++output;
            }
        }
    }
    
    /*
     * reduce: FunctorT must be a binary_function that add two items
     * should be lambda compatible
     */
    
    template<typename IteratorT, typename FunctorT>
    typename IteratorT::value_type reduce(const FunctorT & func, IteratorT it, const IteratorT & end) {
        if (it == end)
            return typename IteratorT::value_type();
        
        typename IteratorT::value_type v(*it);
        
        for(++it; it != end; ++it)
            v = func(v, *it);
        
        return v;
    }
    
    /*
     * Alias for mapping containers and initializer_lists
     */
    
    template<typename ContainerT, typename FunctorT>
    inline typename ContainerT::value_type reduce(const FunctorT & func, const ContainerT & container) {
        return reduce(func, container.begin(), container.end());
    }
    
    /*
     * initializer_list special
     */
    
    template<typename T, typename FunctorT>
    T reduce(const FunctorT & func, std::initializer_list<T> list) {
        if (list.size() == 0)
            return typename std::initializer_list<T>::value_type();
        
        typename std::initializer_list<T>::const_iterator it(list.begin());
        typename std::initializer_list<T>::value_type v(*it);
        
        for(++it; it != list.end(); ++it)
            v = func(v, *it);
        
        return v;
    }
}

#endif /* _CITERTOOLS_CPP11_ */

#endif /* _CITERTOOLS_MAP_H_ */

