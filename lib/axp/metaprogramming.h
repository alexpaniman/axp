#pragma once

#include <concepts>
#include <tuple>

namespace axp {

    template <std::size_t index, typename... types>
    using get_t = std::tuple_element_t<index, std::tuple<types...>>;


    template <auto value>
    struct val {
        static inline auto operator()() {
            return value;
        }
    };

    template <typename type, typename value>
    concept var = std::same_as<decltype(type::operator()()), value>;

}
