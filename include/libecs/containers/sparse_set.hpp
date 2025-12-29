#pragma once

#include <vector>

#include "entity.hpp"

#ifdef LIBECS_SUPPORTED

namespace libecs {
    template <typename T>
    class sparse_set {
    public:
        sparse_set() = default;
        ~sparse_set() = default;

        sparse_set(const sparse_set&) = default;
        sparse_set(sparse_set&&) noexcept = default;

        sparse_set& operator=(const sparse_set&) = default;
        sparse_set& operator=(sparse_set&&) noexcept = default;

        [[nodiscard]] constexpr T& get(size_type index) {
            return dense_[sparse_[index]];
        }

        [[nodiscard]] constexpr const T& get(size_type index) const {
            return dense_[sparse_[index]];
        }

        template <typename... Args>
        constexpr T& emplace(size_type index, Args&&... args) {
            if (index >= sparse_.size()) {
                sparse_.resize(index + 1, static_cast<size_type>(-1));
            }
            else if (sparse_[index] != static_cast<size_type>(-1)) {
                return dense_[sparse_[index]];
            }

            sparse_[index] = dense_.size();
            denseTable_.emplace_back(index);

            return dense_.emplace_back(std::forward<Args>(args)...);
        }

        [[nodiscard]] constexpr bool contains(size_type index) const {
            return index < sparse_.size() && sparse_[index] != static_cast<size_type>(-1);
        }

        constexpr void remove(size_type index) {
            const size_type denseIndex = sparse_[index];
            const size_type last = dense_.size() - 1;

            if (denseIndex != last) {
                dense_[denseIndex] = std::move(dense_[last]);
                denseTable_[denseIndex] = denseTable_[last];
                sparse_[denseTable_[denseIndex]] = denseIndex;
            }

            dense_.pop_back();
            denseTable_.pop_back();
            sparse_[index] = static_cast<size_type>(-1);
        }

        [[nodiscard]] constexpr std::size_t size() const {
            return dense_.size();
        }

        [[nodiscard]] constexpr const std::vector<size_type>& entities() const {
            return denseTable_;
        }

        T* begin() {
            return dense_.begin();
        }

        T* end() {
            return dense_.end();
        }

        const T* begin() const {
            return dense_.begin();
        }

        const T* end() const {
            return dense_.end();
        }

    private:
        std::vector<T> dense_;
        std::vector<size_type> denseTable_;
        std::vector<size_type> sparse_;
    };
}

#endif