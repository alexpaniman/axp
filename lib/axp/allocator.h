#pragma once

#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <malloc.h>
#include <new>
#include <tuple>
    
#include "metaprogramming.h"

namespace axp {

    struct allocated_block {
        void *buffer;
        std::size_t size;
    };

    template <typename impl_type>
    class allocator {
    public:
        template <typename constructor_type, typename... parameter_types>
        constructor_type* create(parameter_types... params) {
            allocated_block block = static_cast<impl_type*>(this)->
                allocate(sizeof(constructor_type));

            :: new (block.buffer) constructor_type(params...);

            return static_cast<constructor_type*>(block.buffer); // TODO: should i lose size?
        }


        template <template <class...> typename constructor_type,
                  typename... parameter_types>
        auto create(parameter_types... params) {
            return create<decltype(constructor_type(params...))>(params...);
        }
    };

    template <typename type>
    concept allocator_like = requires(type allocator, size_t size, allocated_block block) {
        { allocator.allocate(size) } -> std::convertible_to<allocated_block>;

        allocator.deallocate(block);
    };

}


namespace axp::details {

    inline static allocated_block allocate_with_fallback(std::size_t size, const allocator_like auto& current) {
        return current.allocate(size);
    }

    inline static allocated_block allocate_with_fallback(std::size_t size, const allocator_like auto& head,
                                                         const allocator_like auto&... tail_allocators) {

        allocated_block try_current = head.allocate(size);

        if (!try_current.buffer) // Allocation failed, fallback to other methods...
            return allocate_with_fallback(size, tail_allocators...);

        return try_current;      // Allocation succeded, use new space!
    }

    inline static bool owned_by_one_of(const allocated_block& block, const allocator_like auto&... allocators) {
        return (allocators.owns(block) || ...);
    }

}

namespace axp {

    class mallocator: public allocator<mallocator> {
    public:
        allocated_block allocate(std::size_t size) {
            void *buffer = std::malloc(size);
            return { buffer, malloc_usable_size(buffer) };
        };

        void deallocate(const allocated_block &block) {
            free(block.buffer);
        }

        bool good_size(std::size_t size) {
            return size; // TODO: no idea?
        }
    };

    template <typename... allocator_types>
    class fallbackator: public allocator<fallbackator<allocator_types...>> {
    public:
        fallbackator(allocator_types... allocators)
            : m_allocators { std::forward<allocator_types>(allocators)... } {}

        allocated_block allocate(std::size_t size) {
            return std::apply([size](auto&&... allocators) {
                return details::allocate_with_fallback(size, allocators...);
            }, m_allocators);
        }

        std::tuple<allocator_types...> m_allocators;
    };

    inline auto g_mallocator = axp::mallocator {};

}

void* operator new(std::size_t size, axp::allocator_like auto alloc) {
    return alloc.allocate(size).buffer;
}
