#ifndef _CITERTOOLS_ARGMIN_H_
#define _CITERTOOLS_ARGMIN_H_

#include <cit/definition.h>
#include <cit/identity.h>

#ifdef _CITERTOOLS_CPP11_
#include <initializer_list>
#endif /* N _CITERTOOLS_CPP11_ */

/*
 * argmin, takes a measure functor that returns a type that can be used to compare elements
 * the measure *usually* returns a metric of type int, float, double etc
 * but you're free to return any type that implements the < operator
 */

namespace cit {
    #ifdef _CITERTOOLS_CPP11_
    
    template<typename IteratorT, typename MeasureFunctorT = identity<typename IteratorT::value_type> >
    IteratorT argmin(IteratorT && it, const IteratorT & end,
                     const MeasureFunctorT & functor = MeasureFunctorT()) {
        IteratorT best(it++);
        typename MeasureFunctorT::result_type best_value(functor(*best));

        for(; it != end; ++it) {
            typename MeasureFunctorT::result_type value(functor(*it));

            if (value < best_value) {
                best_value = value;
                best = it;
            }
        }

        return best;
    }
    
    template<typename IteratorT, typename MeasureFunctorT = identity<typename IteratorT::value_type> >
    inline IteratorT argmin(const IteratorT & begin, const IteratorT & end,
                            const MeasureFunctorT & functor = MeasureFunctorT()) {
        return argmin(IteratorT(begin), end, functor);
    }
    
    /*
     * For objects that define a begin() and end() method such as std::vector
     * or std::list it is more convenient to use these
     */
    
    template<typename ContainerT, typename MeasureFunctorT = identity<typename ContainerT::value_type> >
    inline typename ContainerT::const_iterator argmin(const ContainerT & container,
                                                      const MeasureFunctorT & functor = MeasureFunctorT()) {
        return argmin<typename ContainerT::const_iterator, MeasureFunctorT>(container.begin(), container.end(), functor);
    }
    
    /*
     * std::initializer_list: C++11 style initializer lists when you want the maximum element of a predefined list
     * *NOTE* This is the *only* version that does *NOT* return an iterator! Because it won't make sense!
     * there won't be a reference to the list as it only exists during the call!
     */
    
    template<typename T, typename MeasureFunctorT = identity<T> >
    inline T argmin(std::initializer_list<T> l, const MeasureFunctorT & functor = MeasureFunctorT()) {
        return *argmin(l.begin(), l.end(), functor);
    }
    
    
    #else
    
    template<typename IteratorT, typename MeasureFunctorT>
    IteratorT argmin(IteratorT it, const IteratorT & end,
                     const MeasureFunctorT & functor) {
        IteratorT best(it++);
        typename MeasureFunctorT::result_type best_value(functor(*best));

        for(; it != end; ++it) {
            typename MeasureFunctorT::result_type value(functor(*it));

            if (value < best_value) {
                best_value = value;
                best = it;
            }
        }

        return best;
    }
    
    /*
     * For objects that define a begin() and end() method such as std::vector
     * or std::list it is more convenient to use these
     */
    
    template<typename ContainerT, typename MeasureFunctorT>
    inline typename ContainerT::const_iterator argmin(const ContainerT & container,
                                                      const MeasureFunctorT & functor) {
        return argmin<typename ContainerT::const_iterator, MeasureFunctorT>(container.begin(), container.end(), functor);
    }
    
    #endif /* N C++11 */
    
    
}

#endif /* N _CITERTOOLS_ARGMAX_H_ */

