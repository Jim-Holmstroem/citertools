#ifndef _CITERTOOLS_COMPOSITION_H_
#define _CITERTOOLS_COMPOSITION_H_

#include <utility>
#include <functional>

/*
 * Composition of two functions
 */

namespace cit {
    template<typename F, typename G>
    class composition : public std::unary_function<typename G::argument_type, typename F::result_type> {
        public:
            composition(const F & f, const G & g) : m_f(f), m_g(g) { }
            inline const typename F::result_type operator() (const typename G::argument_type & value) const {
                return m_f(m_g(value));
            }
        private:
            const F & m_f;
            const G & m_g;
            
    };

    template<typename F, typename G>
    composition<F, G> make_composition(const F & f, const G & g) {
        return composition<F, G>(f, g);
    }
}

#endif /* _CITERTOOLS_IDENTITY_H_ */

