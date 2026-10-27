#pragma once

#ifndef NPC_MOBILE_H
#define NPC_MOBILE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class NPCMobile
{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Clock anim;
    float vitesseX = 80.f;
    float limiteGauche = 550.f;
    float limiteDroite = 800.f;
    sf::Vector2f positionDepart;

    sf::SoundBuffer bufferPapillon;
    sf::Sound sonPapillon{ bufferPapillon };
    bool sonPapillonCharge = false;

public:
    NPCMobile();
    void configurer(sf::Vector2f position, float gauche, float droite);
    void update(float deltaTime);
    sf::FloatRect getBounds() const;
    sf::FloatRect getZonePatrouille() const;
    void recommencer();
    void draw(sf::RenderWindow& window);
};

#endif
