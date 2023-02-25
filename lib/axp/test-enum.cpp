#pragma once

#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

namespace axp::details {
    inline constexpr std::vector<std::string> describe_enum(const std::string data) {

        std::vector<std::string> entries;

        std::string current_word;
        for (std::size_t i = 0; i <= data.size(); ++ i) {
            //                    ^~ last character is '\0'
            //                       included to skip flush
            //                       after the loop's end

            char symbol = data[i];

            if (!std::isalnum(symbol) && symbol != '_') {
                if (!current_word.empty())
                    entries.push_back(current_word);

                continue;
            }

            current_word.push_back(symbol);
        }

        return entries; // list of enum's entries in the order of occurence
    }
};

#define axp_enum(name, ...)                                                         \
    name __VA_ARGS__;                                                               \
                                                                                    \
    const std::vector<std::string> g_##name##_enum_entries =                        \
        axp::details::describe_enum(#__VA_ARGS__);

enum axp_enum(
    hello, {
        a,
        b,
        c
    }
)


int main() {
    for (auto str: g_hello_enum_entries)
        std::cout << str << "\n";
}
