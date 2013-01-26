#ifndef _CITERTOOLS_ARGMAX_H_
#define _CITERTOOLS_ARGMAX_H_

#include <cit/definition.h>

/*
 * argmax
 */

namespace cit {
    #ifdef _CITERTOOLS_CPP11_
    
    template<typename IteratorT, typename HeuristicFunctorT>
    IteratorT argmax(IteratorT && it, const IteratorT & end,
                     const HeuristicFunctorT & functor) {
        IteratorT best(it++);
        typename HeuristicFunctorT::result_type best_value(functor(*best));

        for(; it != end; ++it) {
            typename HeuristicFunctorT::result_type value(functor(*it));

            if (value > best_value) {
                best_value = value;
                best = it;
            }
        }

        return best;
    }
    
    template<typename IteratorT, typename HeuristicFunctorT>
    inline IteratorT argmax(const IteratorT & begin, const IteratorT & end,
                            const HeuristicFunctorT & functor) {
        return argmax(IteratorT(begin), end, functor);
    }
    
    #else
    
    template<typename IteratorT, typename HeuristicFunctorT>
    IteratorT argmax(IteratorT it, const IteratorT & end,
                     const HeuristicFunctorT & functor) {
        IteratorT best(it++);
        typename HeuristicFunctorT::result_type best_value(functor(*best));

        for(; it != end; ++it) {
            typename HeuristicFunctorT::result_type value(functor(*it));

            if (value > best_value) {
                best_value = value;
                best = it;
            }
        }

        return best;
    }
    
    #endif /* N C++11 */
}

#endif /* N _CITERTOOLS_ARGMAX_H_ */

