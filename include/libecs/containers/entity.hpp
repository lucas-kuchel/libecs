#pragma once

#include "../utilities/types.hpp"

#ifdef LIBECS_SUPPORTED

namespace libecs {
    class entity {
    public:
        constexpr entity() = default;

        constexpr entity(size_type id, size_type generation)
            : id_(id), generation_(generation) {
        }

        constexpr ~entity() = default;

        entity(const entity&) = default;
        entity(entity&&) noexcept = default;

        entity& operator=(const entity&) = default;
        entity& operator=(entity&&) noexcept = default;

        constexpr bool operator==(const entity& other) const {
            return id_ == other.id_ && generation_ == other.generation_;
        }

        constexpr bool operator!=(const entity& other) const {
            return id_ != other.id_ || generation_ != other.generation_;
        }

        constexpr size_type id() const {
            return id_;
        }

        constexpr size_type generation() const {
            return generation_;
        }

    private:
        size_type id_ = static_cast<size_type>(-1);
        size_type generation_ = static_cast<size_type>(-1);
    };
}

#endif