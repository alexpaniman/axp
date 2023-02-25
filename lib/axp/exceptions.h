#pragma once

#include "source-location.h"
#include "format.h"

#include <exception>
#include <sstream>

namespace axp {
        
    template <typename... types>
    class exception: public std::exception {
    public:
        exception(const char* format, types... printed_data,
                  axp::source_location location = axp::source_location::current())
            : m_message(location.to_string() + axp::format(format, printed_data...)) {}

        const char* what() const noexcept override {
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };

    template <typename... types>
    exception(const char* format, types... printed_data) -> exception<types...>;

}
