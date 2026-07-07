export module engine:component;

export import :interfaces;

export namespace engine {
    class Component : public IComponent {
    public:
        Component() = default;
        Component(const Component&) = delete;
        Component& operator=(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(Component&&) = delete;
        ~Component() override = default;

        [[nodiscard]] IEntity* entity() noexcept override {
            return _owner;
        }

        [[nodiscard]] const IEntity* entity() const noexcept override {
            return _owner;
        }

        [[nodiscard]] bool enabled() const noexcept override {
            return _enabled;
        }

        [[nodiscard]] bool started() const noexcept override {
            return _started;
        }

        [[nodiscard]] bool destroyQueued() const noexcept {
            return _destroy_queued;
        }

        void setEnabled(bool enabled) override {
            if (_enabled == enabled) {
                return;
            }

            _enabled = enabled;
            if (_enabled) {
                onEnable();
                return;
            }

            onDisable();
        }

        void update(float) override {}

    protected:
        [[nodiscard]] bool hasEntity() const noexcept {
            return _owner != nullptr;
        }

    private:
        friend class Entity;

        void attach(IEntity& _entity) noexcept {
            _owner = &_entity;
            _destroy_queued = false;
            awake();
        }

        void detach() noexcept {
            _owner = nullptr;
        }

        void queueDestroy() noexcept {
            _destroy_queued = true;
        }

        void beginPlay() {
            if (!_started) {
                _started = true;
                start();
            }
        }

        IEntity* _owner = nullptr;
        bool _enabled = true;
        bool _started = false;
        bool _destroy_queued = false;
    };
}
