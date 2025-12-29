#pragma once

#include "macros.hpp"

#ifdef LIBECS_SUPPORTED

#include <cstdint>
#include <cstdlib>

namespace libecs {
    using size_type = std::size_t;
    using int_type = std::uint32_t;
}

namespace libecs::detail {
    [[nodiscard]] inline size_type increment() noexcept {
        static size_type n = 0;

        return n++;
    }
}

namespace libecs {
    template <typename>
    [[nodiscard]] inline size_type type_index() noexcept {
        static size_type n = detail::increment();

        return n;
    }
}

#endif