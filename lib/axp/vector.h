#pragma once

#include <cstddef>
#include <iostream>
#include <memory>

#include "axp-exception.h"
#include "index-out-of-bounds-exception.h"

template <typename element_type>
class vector {
public:
    constexpr vector()
        : m_buffer(new element_type[s_default_capacity]),
          m_capacity(s_default_capacity), m_size(0) {}

    template <typename new_element_type>
    constexpr void push_back(new_element_type&& element) {
        if (m_size >= m_capacity)
            resize(std::max((int) (m_capacity * s_grow_coefficient), m_size + 1));

        :: new(&m_buffer[m_size ++])
               element_type(std::forward<new_element_type>(element));
    }

    constexpr void assert_index_hard(int index) {
        if (index >= m_size || index < 0)
            throw axp::create_exception(index_out_of_bounds_exception, index, m_size);
    }

    #ifndef NDEBUG
    #define ASSERT_INDEX_HARD(...) assert_index_hard(__VA_ARGS__)
    #else
    #define ASSERT_INDEX_HARD(...) ((void) 0)
    #endif

    constexpr       element_type& operator[](int index)        {
        ASSERT_INDEX_HARD(index);
        return m_buffer[index];
    }

    constexpr const element_type& operator[](int index) const  {
        ASSERT_INDEX_HARD(index);
        return m_buffer[index];
    }

    #undef ASSERT_INDEX_HARD

    constexpr       element_type* begin()       noexcept { return m_buffer.get();          }
    constexpr const element_type* begin() const noexcept { return m_buffer.get();          }

    constexpr       element_type*   end()       noexcept { return m_buffer.get() + m_size; }
    constexpr const element_type*   end() const noexcept { return m_buffer.get() + m_size; }


private:
    std::unique_ptr<element_type[]> m_buffer;
    int m_capacity, m_size;

    static constexpr int s_default_capacity = 1;
    static constexpr double s_grow_coefficient = 1.5;

    constexpr void resize(int new_size) {
        std::unique_ptr<element_type[]> new_elements { new element_type[new_size] };

        // Copy all the elements to the new buffer: 
        for (int i = 0; i < std::min(m_size, new_size); ++ i)
            :: new(&new_elements[i]) element_type(std::move(m_buffer[i]));

        m_capacity = new_size;
        m_buffer.reset(new_elements.release());
    }
};
