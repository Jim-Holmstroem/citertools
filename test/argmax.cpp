#include <cit/argmax.h>

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <sys/time.h>

/*
 * Help functions
 */

class IntPairFunctor : public std::unary_function< std::pair<int, int>, int > {
public:
    int operator() (const std::pair<int, int> & v) const {
        return v.first + v.second;
    }
};

std::pair<int, int> rand_pair() {
    return std::make_pair(rand(), rand());
}

int main(int argc, const char **argv) {
    /*
     * Test the cit::argmax
     */
    
    srand(time(NULL));

    std::vector< std::pair<int, int> > ints;

    std::generate_n(std::back_insert_iterator< std::vector< std::pair<int, int> > >(ints), 1000, rand_pair);

    std::vector< std::pair<int, int> >::iterator m (cit::argmax(ints.begin(), ints.end(), IntPairFunctor()));

    std::cout << std::endl << "argmax: " << m->first << ", " << m->second << std::endl;
    
    DEB_PRINTF("debug message");
    WARN_PRINTF("warning message");
    ERR_PRINTF("error message");
    
    /* Simpler call to known std::list, std::vector types that define begin(), end() */
    
    std::vector< std::pair<int, int> >::const_iterator m2 = cit::argmax(ints, IntPairFunctor());
    std::cout << std::endl << "argmax 2: " << m2->first << ", " << m2->second << std::endl;
    
    
#ifdef _CITERTOOLS_CPP11_

    auto m3 = cit::argmax(ints);
    
    std::cout << "argmax 3: " << m3->first << ", " << m3->second << std::endl;
    
    std::cout << "argmax 4: " << cit::argmax({1, 2, 3}) << std::endl;
    
    // backward, i.e. argmin
    std::cout << "argmax 5: " << cit::argmax({1, 2, 3}, std::negate<int>()) << std::endl;

#endif /* N _CITERTOOLS_CPP11_ */
}

