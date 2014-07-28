#include <iostream>
#include <cit/io.h>

struct A {
    A(int a, unsigned int b) : a(a), b(b) {}

    int a;
    unsigned int b;
};

std::ostream & operator<<(std::ostream & stream, const A & a) {
    stream << a.a << ' ' << a.b;
    return stream;
}

int main(int argc, const char **argv) {
    std::cerr << '"' << cit::sprintf("\\ \\\\ \\{ \\{ } } 0} {0} {0} {1} {2} {3} {2} \\{2} \\\\{2}", "Hello", "World", 42, 3.14159) << '"' << std::endl;
    std::cerr << '"' << cit::sprintf("My A-obj: {0}", A(5, 8)) << '"' << std::endl;
    
    cit::fprintf(std::cerr, "Some formatting {0} {1}\n", "Hello", "World");
    cit::printf("Some formatting {0} {1}\n", "Hello", "World");
    
    std::cerr << "joining: " << cit::join(", ", "a", "b", 42, 3.14159) << std::endl;
    std::cerr << "imploding: " << cit::implode(", ", { "a", "b", "c" }) << std::endl;
    return 0;
}

