
#include <cit/map.h>

#include <vector>
#include <list>
#include <iostream>
#include <iterator>
#include <functional>

/*
 * similar to std::plus but can take any number of arguments in operator(...)
 */

class zipper {
    public:
        template<typename T>
        inline T operator() (T t) const {
            std::cerr << t << std::endl;
            return t;
        }
        
        template<typename T, typename... Args>
        inline T operator() (T t, Args... args) const {
            std::cerr << t << " ";
            
            return t + this->operator() (args...);
        }
};

template<int Div>
class is_dividable {
    public:
        inline bool operator() (int x) const {
            return x % Div == 0;
        }
};

int main(int argc, const char **argv) {
    /*
     * Mixing some ranges for arguments to map
     */
    
    const size_t num_elements = 10;
    
    std::vector<int> vector_ints(num_elements);
    std::iota(vector_ints.begin(), vector_ints.end(), 8);
    
    std::vector<int> list_ints(num_elements);
    std::iota(list_ints.begin(), list_ints.end(), 2);
    
    // TODO: C++11 variadic arguments didn't like non-class types
    //const int elements[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    
    /*
     * now do something "funny" with them, do a zip!
     */
    
    std::vector<int> output;
    
    /*
     * map example
     */
    
    cit::map(zipper(), std::back_inserter(output), vector_ints.begin(), vector_ints.end(), list_ints.begin());
    
    std::cerr << "result: ";
    std::copy(output.begin(), output.end(), std::ostream_iterator<int>(std::cerr, " "));
    std::cerr << std::endl;
    
    /*
     * reduce example
     */
    
    std::cerr << "only dividables by 6 results: ";
    cit::filter(is_dividable<6>(), output.begin(), output.end(), std::ostream_iterator<int>(std::cerr, " "));
    std::cerr << std::endl;
    
    std::cerr << "Sum over output: " << cit::reduce(std::plus<int>(), output.begin(), output.end()) << std::endl;
    
    std::cerr << "Multiplies over initializer_list: " << cit::reduce(std::multiplies<int>(), { 1, 2, 3 }) << std::endl;
    
    /*
     * other map example for containers
     */
    
    std::vector<int> output2;
    cit::mapc(zipper(), std::back_inserter(output2), vector_ints, list_ints);

    auto it = cit::mapi(std::function<int(int, int, int)>([] (int x, int y, int z)->int { return x + y + z; }), vector_ints.begin(), vector_ints.end(), list_ints.begin(), list_ints.begin());

    for (; it != it.end(); ++it) {
        std::cerr << "sum: " << *it << std::endl;
    }

    for(int element : cit::mapi(std::function<int(int, int, int)>([] (int x, int y, int z)->int { return x + y + z; }), vector_ints.begin(), vector_ints.end(), list_ints.begin(), list_ints.begin())) {
        std::cerr << element << std::endl;
    }

    /*
     * Yet for the coolest, most "Python-like" syntax :)
     *
     * Lambdas are still ugly though.
     */

    auto lambda = std::function<int(int, int, int)>([] (int x, int y, int z)->int { return x + y + z; });
    //auto lambda = cit::lambda([] (int x, int y, int z)->int { return x + y + z; });

    for(int element : cit::mapic(lambda, vector_ints, list_ints, list_ints)) {
        std::cerr << element << std::endl;
    }

    //auto testit = cit::mapic([] (int x, int y, int z)->int { return x + y + z; }, vector_ints, list_ints, list_ints);
    for(int element : cit::mapic([] (int x, int y, int z) { return x + y + z; }, vector_ints, list_ints, list_ints)) {
        std::cerr << "element: " << element << std::endl;
    }
}

