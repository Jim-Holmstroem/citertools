#include <cit/zip.h>

#include <vector>
#include <cmath>
#include <list>
#include <iostream>
#include <iterator>
#include <functional>

template<typename T>
void print(const T & t){
    std::cout << "[";
    for(const auto & it : t){
        std::cout << it << ",";
    }
    std::cout << "]";
    std::cout << std::endl;
}

template<typename T>
T id(T t){
    return t;
}

int main(int argc, const char **argv){
    auto a = std::list<int>({1,2,3});
    auto b = std::list<int>({4,5});

    print(cit2::zip(a))

    return 0;
}
