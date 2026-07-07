module;

#include <cstddef>
#include <type_traits>

export module engine:interfaces;

export namespace engine {
    class IEntity;

    class IComponent {
    public:
        virtual ~IComponent() = default;

        [[nodiscard]] virtual IEntity* entity() noexcept = 0;
        [[nodiscard]] virtual const IEntity* entity() const noexcept = 0;
        [[nodiscard]] virtual bool enabled() const noexcept = 0;
        [[nodiscard]] virtual bool started() const noexcept = 0;

        virtual void setEnabled(bool enabled) = 0;

        virtual void awake() {}
        virtual void start() {}
        virtual void update(float delta_time) = 0;
        virtual void onEnable() {}
        virtual void onDisable() {}
        virtual void onDestroy() {}
    };

    class IEntity {
    public:
        virtual ~IEntity() = default;

        [[nodiscard]] virtual std::size_t id() const noexcept = 0;
        [[nodiscard]] virtual bool active() const noexcept = 0;

        virtual void setActive(bool active) = 0;
        virtual void update(float delta_time) = 0;
        virtual void destroy() = 0;

        template <typename T>
        [[nodiscard]] T* as() noexcept {
            static_assert(std::is_base_of_v<IEntity, T>, "T must derive from IEntity");
            return dynamic_cast<T*>(this);
        }

        template <typename T>
        [[nodiscard]] const T* as() const noexcept {
            static_assert(std::is_base_of_v<IEntity, T>, "T must derive from IEntity");
            return dynamic_cast<const T*>(this);
        }
    };
}
