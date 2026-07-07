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
    class Entity : public IEntity {
    public:
        Entity() : _id(_next_id++) {}

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;
        Entity(Entity&&) = delete;
        Entity& operator=(Entity&&) = delete;

        ~Entity() override {
            destroy();
        }

        [[nodiscard]] std::size_t id() const noexcept override {
            return _id;
        }

        [[nodiscard]] bool active() const noexcept override {
            return _active;
        }

        [[nodiscard]] bool destroyQueued() const noexcept {
            return _destroy_queued;
        }

        void setActive(bool active) override {
            _active = active;
        }

        template <typename T, typename... Args>
        T& addComponent(Args&&... _args) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from engine::Component");

            auto _component = std::make_unique<T>(std::forward<Args>(_args)...);
            auto* _raw = _component.get();

            _raw->attach(*this);
            _components.push_back(std::move(_component));
            return *_raw;
        }

        template <typename T>
        [[nodiscard]] T* getComponent() noexcept {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from engine::Component");

            for (const auto& _component : _components) {
                if (auto* _typed = dynamic_cast<T*>(_component.get())) {
                    return _typed;
                }
            }

            return nullptr;
        }

        template <typename T>
        [[nodiscard]] const T* getComponent() const noexcept {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from engine::Component");

            for (const auto& _component : _components) {
                if (auto* _typed = dynamic_cast<const T*>(_component.get())) {
                    return _typed;
                }
            }

            return nullptr;
        }

        template <typename T>
        [[nodiscard]] bool hasComponent() const noexcept {
            return getComponent<T>() != nullptr;
        }

        template <typename T>
        bool removeComponent() {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from engine::Component");

            const auto _iterator = std::find_if(_components.begin(), _components.end(), [](const auto& _component) {
                return dynamic_cast<T*>(_component.get()) != nullptr;
            });

            if (_iterator == _components.end()) {
                return false;
            }

            (*_iterator)->onDestroy();
            (*_iterator)->detach();
            _components.erase(_iterator);
            return true;
        }

        template <typename T>
        bool queueRemoveComponent() {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from engine::Component");

            if (auto* _component = getComponent<T>()) {
                return queueRemoveComponent(*_component);
            }

            return false;
        }

        bool queueRemoveComponent(Component& _component) noexcept {
            if (_component.entity() != this || _component.destroyQueued()) {
                return false;
            }

            _component.queueDestroy();
            _components_to_destroy.push_back(&_component);
            return true;
        }

        void queueDestroy() noexcept {
            _destroy_queued = true;
        }

        void update(float delta_time) override {
            processComponentDestroyQueue();

            if (!_active || _destroyed) {
                return;
            }

            for (const auto& _component : _components) {
                if (_component->destroyQueued() || !_component->enabled()) {
                    continue;
                }

                _component->beginPlay();
                _component->update(delta_time);
            }
        }

        void destroy() override {
            if (_destroyed) {
                return;
            }

            _destroyed = true;
            _destroy_queued = false;
            for (auto& _component : _components) {
                _component->onDestroy();
                _component->detach();
            }
            _components.clear();
            _components_to_destroy.clear();
        }

    private:
        void processComponentDestroyQueue() {
            if (_components_to_destroy.empty()) {
                return;
            }

            for (auto* _component : _components_to_destroy) {
                if (_component->entity() == this) {
                    _component->onDestroy();
                    _component->detach();
                }
            }

            std::erase_if(_components, [](const auto& _component) {
                return _component->destroyQueued();
            });

            _components_to_destroy.clear();
        }

        inline static std::size_t _next_id = 1;

        std::size_t _id = 0;
        bool _active = true;
        bool _destroyed = false;
        bool _destroy_queued = false;
        std::vector<std::unique_ptr<Component>> _components;
        std::vector<Component*> _components_to_destroy;
    };
}
