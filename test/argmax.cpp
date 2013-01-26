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
}

