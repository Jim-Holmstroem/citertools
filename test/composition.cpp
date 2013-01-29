#include <cit/composition.h>

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <sys/time.h>
#include <functional>


/*
 * Help functions
 */

int main(int argc, const char **argv) {
    /*
     * Test the cit::composition
     */
    
    auto nn = cit::composition(&std::negate,&std::negate); 
    std::cout << nn(1) << std::endl;


}

