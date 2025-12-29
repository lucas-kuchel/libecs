#pragma once

#include "../utilities/types.hpp"
#include "storage.hpp"

#ifdef LIBECS_SUPPORTED

namespace libecs {
    class component_storage {
    public:
        component_storage() = default;
        ~component_storage() = default;

        component_storage(const component_storage&) = default;
        component_storage(component_storage&&) noexcept = default;

        component_storage& operator=(const component_storage&) = default;
        component_storage& operator=(component_storage&&) noexcept = default;

        [[nodiscard]] constexpr sparse_set_storage& operator[](std::size_t index) {
            return storage_[index];
        }

        [[nodiscard]] constexpr const sparse_set_storage& operator[](std::size_t index) const {
            return storage_[index];
        }

        template <typename T>
        [[nodiscard]] constexpr sparse_set<T>& acquire() {

            std::size_t index = type_index<T>();

            if (index >= storage_.size()) {
                storage_.resize(index + 1);
            }

            sparse_set_storage& container = storage_[index];

            [[unlikely]] if (container.needs_init()) {
                container.init<T>();
            }

            return container.get<T>();
        }

        std::vector<sparse_set_storage>::iterator begin() {
            return storage_.begin();
        }

        std::vector<sparse_set_storage>::iterator end() {
            return storage_.end();
        }

    private:
        std::vector<sparse_set_storage> storage_;
    };
}

#endif