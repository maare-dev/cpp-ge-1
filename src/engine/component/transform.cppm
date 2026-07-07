module;

#include <iostream>

export module engine:transform;

import :component;

export namespace engine {
    class Transform final : public Component {
    public:
        Transform() = default;

        Transform(float x, float y, float velocity_x = 0.0F, float velocity_y = 0.0F)
            : _x(x),
              _y(y),
              _velocity_x(velocity_x),
              _velocity_y(velocity_y) {}

        void start() override {
            std::cout << "Transform::start -> position(" << _x << ", " << _y << ")\n";
        }

        void update(float delta_time) override {
            _x += _velocity_x * delta_time;
            _y += _velocity_y * delta_time;

            std::cout << "Transform::update -> dt=" << delta_time
                      << " position(" << _x << ", " << _y << ")\n";
        }

        [[nodiscard]] float x() const noexcept {
            return _x;
        }

        [[nodiscard]] float y() const noexcept {
            return _y;
        }

        void setPosition(float x, float y) noexcept {
            _x = x;
            _y = y;
        }

        void translate(float x, float y) noexcept {
            _x += x;
            _y += y;
        }

        void setVelocity(float velocity_x, float velocity_y) noexcept {
            _velocity_x = velocity_x;
            _velocity_y = velocity_y;
        }

    private:
        float _x = 0.0F;
        float _y = 0.0F;
        float _velocity_x = 0.0F;
        float _velocity_y = 0.0F;
    };
}
