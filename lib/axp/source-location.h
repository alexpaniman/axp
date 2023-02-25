#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <source_location>

namespace axp {

    struct source_location {
        const char* file;
        const char* function;

        std::size_t line, column;

        static source_location current(const char* file = __builtin_FILE(),
                                       const char* function = __builtin_FUNCTION(),
                                       std::size_t line = __builtin_LINE(),
                                       std::size_t column = __builtin_COLUMN()) {

            return { file, function, line, column };
        }

        friend std::ostream& operator<<(std::ostream &os, const source_location &location) {
            os << "In "  << location.file
               << ":"    << location.line << ":" << location.column
               << " at " << location.function << "\n";

            return os;
        }

        std::string to_string() const {
            std::stringstream ss;
            ss << *this;

            return ss.str();
        }

    };

}
