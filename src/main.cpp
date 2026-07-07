import engine;

#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <iostream>
#include "engine/define/info.hpp"

int main(){
    engine::Scene _scene;
    auto& _entity = _scene.addEntity();
    _entity.addComponent<engine::Transform>(0.0F, 0.0F, 32.0F, 16.0F);
    auto* _transform = _entity.getComponent<engine::Transform>();

    std::cout << "Demo begin\n";
    _scene.update(0.25F);
    _scene.update(0.75F);
    std::cout << "Final position: (" << _transform->x() << ", " << _transform->y() << ")\n";

    sf::RenderWindow window(sf::VideoMode({800, 600}), ENGINE_NAME);

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            //TODO: give each event to scene
            //TODO: delete this event checking
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
        //TODO: update scene
        window.clear();
        //TODO: draw scene
        window.display();
    }
    return 0;
}
