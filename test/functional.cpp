#include <cit/functional.h>
#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, const char **argv) {
    std::vector<int> ints({1, 2, 3});
    
    //std::for_each(ints.begin(), ints.end(), cit::unary_lambda<int, int>([] (int x) { std::cerr << x << std::endl; }));
}

