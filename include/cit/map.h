#ifndef _CITERTOOLS_MAP_H_
#define _CITERTOOLS_MAP_H_

#include <cit/definition.h>
#include <numeric>
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

