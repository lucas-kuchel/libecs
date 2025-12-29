#pragma once

#include "components.hpp"

#ifdef LIBECS_SUPPORTED

namespace libecs {
    template <typename... Args>
    class view {
    public:
        view(component_storage& storage, std::vector<size_type>& generations)
            : storage_(&storage), generations_(&generations) {
            smallest_ = (pick_smallest(smallest_, type_index<Args>()), ...);
        }

        class iterator {
        public:
            iterator(view* v, std::size_t position)
                : view_(v), position_(position) {
                advance_to_valid();
            }

            [[nodiscard]] constexpr entity operator*() const {
                size_type id = view_->smallest_->entities()[position_];

                return {id, (*view_->generations_)[id]};
            }

            constexpr iterator& operator++() {
                position_++;

                advance_to_valid();

                return *this;
            }

            [[nodiscard]] constexpr bool operator==(const iterator& other) const {
                return position_ == other.position_;
            }

            [[nodiscard]] constexpr bool operator!=(const iterator& other) const {
                return position_ != other.position_;
            }

        private:
            void advance_to_valid() {
                while (position_ < view_->smallest_->size()) {
                    size_type entity = view_->smallest_->entities()[position_];

                    if (view_->has_all_components(entity)) {
                        break;
                    }

                    position_++;
                }
            }

            view* view_;
            std::size_t position_;
        };

        class each_iterator {
        public:
            each_iterator(view* view, std::size_t position)
                : view_(view), position_(position) {
                advance_to_valid();
            }

            [[nodiscard]] auto operator*() {
                size_type id = view_->smallest_->entities()[position_];

                return std::tuple<entity, Args&...>({id, (*view_->generations_)[id]}, view_->get_component_ref<Args>(id)...);
            }

            each_iterator& operator++() {
                position_++;

                advance_to_valid();

                return *this;
            }

            [[nodiscard]] bool operator==(const each_iterator& o) const {
                return position_ == o.position_;
            }

            [[nodiscard]] bool operator!=(const each_iterator& o) const {
                return position_ != o.position_;
            }

        private:
            void advance_to_valid() {
                while (position_ < view_->smallest_->size()) {
                    size_type entity = view_->smallest_->entities()[position_];

                    if (view_->has_all_components(entity)) {
                        break;
                    }

                    position_++;
                }
            }

            view* view_;
            std::size_t position_;
        };

        class each_range {
        public:
            each_range(view* view)
                : view_(view) {
            }

            each_iterator begin() {
                return each_iterator(view_, 0);
            }

            each_iterator end() {
                return each_iterator(view_, view_->smallest_->size());
            }

        private:
            view* view_;
        };

        iterator begin() {
            return iterator(this, 0);
        }

        iterator end() {
            return iterator(this, smallest_->size());
        }

        each_range each() {
            return each_range{this};
        }

    private:
        [[nodiscard]] bool has_all_components(size_type entity) const {
            return (has_component<Args>(entity) && ...);
        }

        template <typename T>
        [[nodiscard]] bool has_component(size_type entity) const {
            auto& pool = (*storage_)[type_index<T>()];
            return pool.contains(entity);
        }

        template <typename T>
        [[nodiscard]] constexpr T& get_component_ref(size_type e) {
            auto& pool = (*storage_)[type_index<T>()];
            return pool.template get<T>().get(e);
        }

        constexpr sparse_set_storage* pick_smallest(sparse_set_storage* current, std::size_t index) {
            sparse_set_storage& idx = (*storage_)[index];

            if (current == nullptr || current->size() > idx.size()) {
                return &idx;
            }

            return current;
        }

        std::vector<size_type>* generations_ = nullptr;
        component_storage* storage_ = nullptr;
        sparse_set_storage* smallest_ = nullptr;
    };
}

#endif