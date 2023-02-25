#pragma once

#include <iostream>
#include <ostream>
#include <sstream>

namespace details {

    void format_one_arg(std::ostream &os, const char*& format, auto&& arg) {
        while (format[0] != '{' || format[1] != '}') {
            os << *format; ++ format;
        }

        os << std::forward<decltype(arg)>(arg); format += 2; // Advance to symbol after {}
    }

}

namespace axp {

    void print(std::ostream &os, const char* format, auto&&... args) {
        (details::format_one_arg(os, format, std::forward<decltype(args)>(args)), ...);
        while (*format != '\0')
            os << *(format ++); // Write out final piece of string
    }

    void print(const char* format, auto&&... args) {
        print(std::cout, format, args...);
    }

    static std::string format(const char* format, auto&&... args) {
        std::stringstream ss;
        print(ss, format, args...);

        return ss.str();
    }

}
