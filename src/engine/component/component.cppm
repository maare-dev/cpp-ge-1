export module engine:component;

export import :interfaces;

export namespace engine {
    class Component : public Icomponent {
    public:
        Component() = default;
        Component(const Component&) = delete;
        Component& operator=(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(Component&&) = delete;
        ~Component() override = default;

        [[nodiscard]] IEntity* entity() noexcept override {
            return owner_;
        }

        [[nodiscard]] const IEntity* entity() const noexcept override {
            return owner_;
        }

        [[nodiscard]] bool enabled() const noexcept override {
            return enabled_;
        }

        [[nodiscard]] bool started() const noexcept override {
            return started_;
        }

        void set_enabled(bool value) override {
            if (enabled_ == value) {
                return;
            }

            enabled_ = value;
            if (enabled_) {
                on_enable();
                return;
            }

            on_disable();
        }

        void update(float) override {}

    protected:
        [[nodiscard]] bool has_entity() const noexcept {
            return owner_ != nullptr;
        }

    private:
        friend class Entity;

        void attach(IEntity& entity) noexcept {
            owner_ = &entity;
            awake();
        }

        void detach() noexcept {
            owner_ = nullptr;
        }

        void begin_play() {
            if (!started_) {
                started_ = true;
                start();
            }
        }

        IEntity* owner_ = nullptr;
        bool enabled_ = true;
        bool started_ = false;
    };
}
