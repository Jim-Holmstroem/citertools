#ifndef _CITER_IO_H_
#define _CITER_IO_H_

#include <array>
#include <cstdlib>
#include <string>
#include <sstream>

#include <cit/definition.h>

#ifndef _CITERTOOLS_CPP11_
#error "Nothing in <cit/io.h> can be used when not using C++11!"
#else

namespace cit {
    namespace internal {
        template<typename T>
        inline std::string to_string(const T & obj) {
            std::stringstream ss;
            ss << obj;
            return ss.str();
        }
    }

    /*
     * Typesafe printf-version using variadic template arguments.
     * enables indexed references to arguments etc.
     *
     * example format: "{0} is the first argument, {1} is the second, {1} is the second again. \\{ is a left bracket."
     *
     * to print custom objects, override your std::ostream << operator just as you would for std::cout/std::cerr.
     */
    template<typename... Args>
    std::ostream & fprintf(std::ostream & stream, const char *fmt, Args... args) {
        std::array<std::string, sizeof...(Args)> str_args{ { internal::to_string(args)... } };
        for (const char *it = fmt; *it != '\0'; ++it) {
            if (it[0] == '{') {
                char *end;
                const long index(std::strtol(&it[1], &end, 10));
                if (end != &it[1]) {
                    it = end;
                    stream << str_args[((unsigned long) index) % str_args.size()];
                } else
                    stream << '{';
            } else if (it[0] == '\\') {
                if (it[1] == '{') {
                    stream << '{';
                    it += 1;
                } else if (it[1] == '\\') {
                    stream << '\\';
                    it += 1;
                } else
                    stream << '\\';
            } else
                stream << *it;
        }

        return stream;
    }

    template<typename... Args>
    inline std::string sprintf(const char *fmt, Args... args) {
        std::stringstream ss;
        fprintf(ss, fmt, args...);
        return ss.str();
    }

    template<typename... Args>
    inline std::ostream & printf(const char *fmt, Args... args) {
        return fprintf(std::cout, fmt, args...);
    }

    /*
     * Can be done with std::copy, std::ostream_iterator but this might be more convenient.
     * Also i think the standard adds a trailing separator which is ugly.
     */
    template<typename IteratorT>
    std::string implode(const char *separator, IteratorT it, const IteratorT & end) {
        std::stringstream ss;
        if (it != end) {
            ss << *it;
            for (++it; it != end; ++it)
                ss << separator << *it;
        }

        return ss.str();
    }

    template<typename ContainerT>
    inline std::string implode(const char *separator, const ContainerT & container) {
        return implode(separator, container.begin(), container.end());
    }
    
    template<typename T>
    inline std::string implode(const char *separator, const std::initializer_list<T> & container) {
        return implode(separator, container.begin(), container.end());
    }

    namespace internal {
        template<typename Arg>
        void join(const char *separator, std::stringstream & ss, Arg & arg) {
            ss << arg;
        }

        template<typename Arg1, typename Arg2, typename... Args>
        void join(const char *separator, std::stringstream & ss, Arg1 & arg1, Arg2 & arg2, Args&... args) {
            ss << arg1 << separator;
            join(separator, ss, arg2, args...);
        }
    }

    template<typename... Args>
    std::string join(const char *separator, Args... args) {
        std::stringstream ss;
        internal::join(separator, ss, args...);
        return ss.str();
    }
}

#endif /* _CITERTOOLS_CPP11_ */

#endif /* _CPP11_STRING_PRINTF_H_ */

