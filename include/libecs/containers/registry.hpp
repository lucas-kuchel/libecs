#pragma once

#include "components.hpp"
#include "entity.hpp"
#include "sparse_set.hpp"
#include "storage.hpp"
#include "view.hpp"

#ifdef LIBECS_SUPPORTED

namespace libecs {
    class registry {
    public:
        registry() = default;
        ~registry() = default;

        registry(const registry&) = delete;
        registry(registry&&) noexcept = default;

        registry& operator=(const registry&) = delete;
        registry& operator=(registry&&) noexcept = default;

        [[nodiscard]] constexpr entity create() {
            size_type id = 0;
            size_type generation = 0;

            if (freeList_.empty()) {
                id = generations_.size();
                generation = generations_.emplace_back(0);
            }
            else {
                id = freeList_.back();
                generation = generations_[id];
                freeList_.pop_back();
            }

            return {id, generation};
        }

        [[nodiscard]] constexpr bool valid(entity entity) const {
            return entity.id() < generations_.size() && generations_[entity.id()] == entity.generation();
        }

        constexpr void destroy(entity entity) {
            freeList_.emplace_back(entity.id());
            generations_[entity.id()]++;

            for (auto& container : storage_) {
                container.remove(entity.id());
            }
        }

        template <typename T, typename... Args>
        constexpr T& emplace(entity entity, Args&&... args) {
            sparse_set<T>& container = storage_.acquire<T>();

            return container.emplace(entity.id(), std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        [[nodiscard]] constexpr bool contains(entity entity) const {
            sparse_set<T>& container = storage_.acquire<T>();

            return container.contains(entity.id());
        }

        template <typename T>
        [[nodiscard]] constexpr T& get(entity entity) {
            sparse_set<T>& container = storage_.acquire<T>();

            return container.get(entity.id());
        }

        template <typename T>
        [[nodiscard]] constexpr const T& get(entity entity) const {
            sparse_set<T>& container = storage_.acquire<T>();

            return container.get(entity.id());
        }

        template <typename T>
        constexpr void remove(entity entity) {
            sparse_set<T>& container = storage_.acquire<T>();

            return container.remove(entity.id());
        }

        template <typename... Args>
        [[nodiscard]] constexpr libecs::view<Args...> view() {
            return libecs::view<Args...>(storage_, generations_);
        }

    private:
        std::vector<size_type> generations_;
        std::vector<size_type> freeList_;
        component_storage storage_;
    };
}

#endif