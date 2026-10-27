#pragma once

#ifndef NPC_FIXE_H
#define NPC_FIXE_H

#include <SFML/Graphics.hpp>

class NPCFixe
{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Clock anim;

public:
    NPCFixe();
    void configurer(sf::Vector2f position);
    void update(float deltaTime);
    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow& window);
};

#endif