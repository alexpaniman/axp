#include "source-location.h"
#include <ostream>
#include <vector>

template <typename value_type>
std::ostream &operator<<(std::ostream &os, std::vector<value_type> &vec) {

    os << "[ ";
    for (const auto &element: vec)
        os << element << " ";
    os << "]";

    return os;
}

#include "exceptions.h"
#include "format.h"

int main() {
    // hello_world x;
    // x[0]; // [0]

    // std::vector<int> ints = { 1, 2, 3, 4, 5 };
    // throw axp::exception("I failed because {} overflowed!", ints);
}
