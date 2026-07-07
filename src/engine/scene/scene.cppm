module;

#include <algorithm>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

export module engine:scene;

import :entity;

export namespace engine {
    class Scene {
    public:
        Scene() = default;
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(Scene&&) = delete;
        virtual ~Scene() = default;

        template <typename T = Entity, typename... Args>
        T& addEntity(Args&&... _args) {
            static_assert(std::is_base_of_v<Entity, T>, "T must derive from engine::Entity");

            auto _entity = std::make_unique<T>(std::forward<Args>(_args)...);
            auto* _raw = _entity.get();

            _entities.push_back(std::move(_entity));
            return *_raw;
        }

        template <typename T>
        [[nodiscard]] T* getEntity() noexcept {
            static_assert(std::is_base_of_v<Entity, T>, "T must derive from engine::Entity");

            for (const auto& _entity : _entities) {
                if (auto* _typed = dynamic_cast<T*>(_entity.get())) {
                    return _typed;
                }
            }

            return nullptr;
        }

        template <typename T>
        [[nodiscard]] const T* getEntity() const noexcept {
            static_assert(std::is_base_of_v<Entity, T>, "T must derive from engine::Entity");

            for (const auto& _entity : _entities) {
                if (auto* _typed = dynamic_cast<const T*>(_entity.get())) {
                    return _typed;
                }
            }

            return nullptr;
        }

        template <typename T>
        [[nodiscard]] bool hasEntity() const noexcept {
            return getEntity<T>() != nullptr;
        }

        bool queueRemoveEntity(Entity& _entity) noexcept {
            if (_entity.destroyQueued()) {
                return false;
            }

            _entity.queueDestroy();
            _entities_to_destroy.push_back(&_entity);
            return true;
        }

        template <typename T>
        bool queueRemoveEntity() noexcept {
            if (auto* _entity = getEntity<T>()) {
                return queueRemoveEntity(*_entity);
            }

            return false;
        }

        void update(float delta_time) {
            for (const auto& _entity : _entities) {
                auto* _typed = static_cast<Entity*>(_entity.get());
                if (_typed->destroyQueued() || !_typed->active()) {
                    continue;
                }

                _typed->update(delta_time);
            }

            processEntityDestroyQueue();
        }

        void clear() {
            for (auto& _entity : _entities) {
                _entity->destroy();
            }

            _entities.clear();
            _entities_to_destroy.clear();
        }

    protected:
        void processEntityDestroyQueue() {
            if (_entities_to_destroy.empty()) {
                return;
            }

            for (auto* _entity : _entities_to_destroy) {
                _entity->destroy();
            }

            std::erase_if(_entities, [](const auto& _entity) {
                return static_cast<Entity*>(_entity.get())->destroyQueued();
            });

            _entities_to_destroy.clear();
        }

        std::vector<std::unique_ptr<Entity>> _entities;

    private:
        std::vector<Entity*> _entities_to_destroy;
    };
}
