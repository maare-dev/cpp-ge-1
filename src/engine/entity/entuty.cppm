module;

#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

export module engine:entity;

import :component;

export namespace engine {
    class Entity final : public IEntity {
    public:
        Entity() : id_(next_id_++) {}

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;
        Entity(Entity&&) = delete;
        Entity& operator=(Entity&&) = delete;

        ~Entity() override {
            destroy();
        }

        [[nodiscard]] std::size_t id() const noexcept override {
            return id_;
        }

        [[nodiscard]] bool active() const noexcept override {
            return active_;
        }

        void set_active(bool value) override {
            active_ = value;
        }

        template <typename T, typename... Args>
        T& add_component(Args&&... args) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from engine::Component");

            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            auto* raw = component.get();

            raw->attach(*this);
            components_.push_back(std::move(component));
            return *raw;
        }

        template <typename T>
        [[nodiscard]] T* get_component() noexcept {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from engine::Component");

            for (const auto& component : components_) {
                if (auto* typed = dynamic_cast<T*>(component.get())) {
                    return typed;
                }
            }

            return nullptr;
        }

        template <typename T>
        [[nodiscard]] const T* get_component() const noexcept {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from engine::Component");

            for (const auto& component : components_) {
                if (auto* typed = dynamic_cast<const T*>(component.get())) {
                    return typed;
                }
            }

            return nullptr;
        }

        template <typename T>
        [[nodiscard]] bool has_component() const noexcept {
            return get_component<T>() != nullptr;
        }

        template <typename T>
        bool remove_component() {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from engine::Component");

            const auto it = std::find_if(components_.begin(), components_.end(), [](const auto& component) {
                return dynamic_cast<T*>(component.get()) != nullptr;
            });

            if (it == components_.end()) {
                return false;
            }

            (*it)->on_destroy();
            (*it)->detach();
            components_.erase(it);
            return true;
        }

        void update(float delta_time) override {
            if (!active_) {
                return;
            }

            for (const auto& component : components_) {
                if (!component->enabled()) {
                    continue;
                }

                component->begin_play();
                component->update(delta_time);
            }
        }

        void destroy() override {
            if (destroyed_) {
                return;
            }

            destroyed_ = true;
            for (auto& component : components_) {
                component->on_destroy();
                component->detach();
            }
            components_.clear();
        }

    private:
        inline static std::size_t next_id_ = 1;

        std::size_t id_ = 0;
        bool active_ = true;
        bool destroyed_ = false;
        std::vector<std::unique_ptr<Component>> components_;
    };
}
