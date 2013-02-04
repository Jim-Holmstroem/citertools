#include <cit/argmin.h>

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
     * Test the cit::argmin
     */
    
    srand(time(NULL));

    std::vector< std::pair<int, int> > ints;

    std::generate_n(std::back_insert_iterator< std::vector< std::pair<int, int> > >(ints), 1000, rand_pair);

    std::vector< std::pair<int, int> >::iterator m (cit::argmin(ints.begin(), ints.end(), IntPairFunctor()));

    std::cout << std::endl << "argmin: " << m->first << ", " << m->second << std::endl;
    
    DEB_PRINTF("debug message");
    WARN_PRINTF("warning message");
    ERR_PRINTF("error message");
    
    /* Simpler call to known std::list, std::vector types that define begin(), end() */
    
    std::vector< std::pair<int, int> >::const_iterator m2 = cit::argmin(ints, IntPairFunctor());
    std::cout << std::endl << "argmin 2: " << m2->first << ", " << m2->second << std::endl;
    
    
#ifdef _CITERTOOLS_CPP11_

    auto m3 = cit::argmin(ints);
    
    std::cout << "argmin 3: " << m3->first << ", " << m3->second << std::endl;
    
    std::cout << "argmin 4: " << cit::argmin({1, 2, 3}) << std::endl;
    
    // backward, i.e. argmax
    std::cout << "argmin 5: " << cit::argmin({1, 2, 3}, std::negate<int>()) << std::endl;

#endif /* N _CITERTOOLS_CPP11_ */
}

