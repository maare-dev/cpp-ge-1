#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include "engine/define/info.hpp"

import engine.scene.Scene;

int main(){
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
