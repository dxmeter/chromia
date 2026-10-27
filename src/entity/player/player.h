#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <filesystem>

#include "../../level/level.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

    float vitesseX = 0.f;
    float vitesseY = 0.f;
    float gravite = 800.f;
    float forceSaut = 550.f;
    bool onFloor = false;
    bool spaceAvant = false;
    bool isInvicible = false;
    sf::Clock chronoDegats;

    int vie = 3;

    sf::Vector2f positionDepart;
    sf::Vector2f positionDebutNiveau;
    sf::Clock anim;

    bool bonusVitesseActif = false;
    sf::Clock chronoBonusVitesse;

    sf::SoundBuffer bufferSaut;
    sf::Sound sonSaut{ bufferSaut };
    bool sonSautCharge = false;

    sf::SoundBuffer bufferDegat;
    sf::Sound sonDegat{ bufferDegat };
    bool sonDegatCharge = false;

    sf::SoundBuffer bufferMarche;
    sf::Sound sonMarche{ bufferMarche };
    bool sonMarcheCharge = false;

public:
    Player();

    void update(float deltaTime1, bool isMoving, const Level& level);

    bool touchePlateforme(const Level& level, float direction, bool horizontal) const;
    void moveWithCollision(float distance, bool horizontal, const Level& level);
    sf::FloatRect getBounds() const;
    void memoPosition(const Level& level);

    sf::Vector2f getCameraTarget() const;
    
    void perdreVie();
    void gagnerVie();
    int getVies() const;

    void activerBonusVitesse();

    void recommencer();

    void definirDepart(sf::Vector2f position);

    void draw(sf::RenderWindow& window);
};

#endif