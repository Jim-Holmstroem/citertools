#ifndef _CITERTOOLS_COMPOSITION_H_
#define _CITERTOOLS_COMPOSITION_H_

#include <utility>
#include <functional>

/*
 * Composition of two functions
 */

namespace cit {
    template<typename F,typename G>
    class composition : public std::unary_function<typename G::argument_type, typename F::result_type> {
        public:
            inline const typename F::result_type & operator() (const typename G::argument_type & value) const {
                return F(G(value));
            }
            
    };


}

#endif /* _CITERTOOLS_IDENTITY_H_ */

