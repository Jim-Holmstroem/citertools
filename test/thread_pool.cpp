#include <cit/thread/pool.h>

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h> // For sleep

int main(int argc, const char **argv) {
    cit::thread::pool_t pool;
    
    std::future<int> future_int = pool.push([](int a, int b, int c) { sleep(1); return a + b + c; }, 42, 1337, 17);

    std::future<float> future_float = pool.push([](float radians) { sleep(5); return radians * 180 / 3.14159f; }, 3.14159f);

    std::future<std::string> future_string = pool.push([](int a, int b, int c) { sleep(5); std::stringstream ss; ss << a << ' ' << b << ' ' << c; return ss.str(); }, 42, 1337, 17);

    std::cerr << "Waiting for future int: ";
    std::cerr << future_int.get() << std::endl;

    std::cerr << "Waiting for any results..." << std::endl;
    pool.wait_idle();
    std::cerr << "Non-blocking results: future_float: " << future_float.get() << ", future_string: " << future_string.get() << std::endl;
    
    // Making sure references work too.
    
    int number = 42;
    std::future<int> future_int2 = pool.push([](int & x) { int copy = x; ++x; return copy; }, std::ref(number));
    std::cerr << "future_int2: " << future_int2.get();
    std::cerr << ", number: " << number << std::endl;
    
    // And make sure not referencing works too!
    // Notice that the method still takes a reference, this is faster, it's the same behavior as with std::bind.
    // The real difference is the use of std::ref as shown above.
    
    std::vector<int> rands(100);
    std::generate(rands.begin(), rands.end(), rand);
    std::future<int> future_int3 = pool.push([](std::vector<int> & copy) { sleep(1); return int(copy.size()); }, rands);
    rands.clear();
    std::cerr << "future_int3: " << future_int3.get() << std::endl;
    
    return 0;
}

