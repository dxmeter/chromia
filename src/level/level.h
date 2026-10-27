#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include <filesystem>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Image.hpp"

#include "../entity/npc/npc_mobile.h"
#include "../entity/npc/npc_fixe.h"
#include "../entity/collectible/collectible.h"
#include "../entity/bonus/bonus.h"

class Level
{
private:
    sf::Texture backgroundTexture;
    sf::Sprite background;
    bool backgroundCharge = false;

    sf::Texture platformsTexture;
    sf::Sprite platforms;
    sf::Image collisionMask;

    sf::Texture porteTexture;
    sf::Sprite porte;

    int nombreFixes = 10;
    int nombreMobiles = 4;
    NPCMobile ennemisMobiles[4];
    NPCFixe ennemisFixes[10];

    Collectible collectibles[7];

    Bonus bonusVie;
    Bonus bonusVitesse[2];

    sf::SoundBuffer bufferCollecte;
    sf::Sound sonCollecte{ bufferCollecte };
    bool sonCollecteCharge = false;

public:
    Level();
    bool charger(int numero);

    void updateMobiles(float deltaTime);
    void drawMobiles(sf::RenderWindow& window);
    bool toucheMobile(sf::FloatRect rectangle) const;
    sf::FloatRect getMobileBounds(int indice) const;

    void updateFixes(float deltaTime);
    void drawFixes(sf::RenderWindow& window);
    bool toucheFixe(sf::FloatRect rectangle) const;
    sf::FloatRect getFixeBounds(int indice) const;

    void updateCollectibles(float deltaTime);
    void drawCollectibles(sf::RenderWindow& window);
    sf::Color recupererCollectibles(sf::FloatRect joueur, sf::Color couleurActuelle);
    void recommencerCollectibles();
    bool tousCollectiblesRecuperes() const;

    bool recupererBonusVie(sf::FloatRect joueur);
    bool recupererBonusVitesse(sf::FloatRect joueur);

    sf::FloatRect getPorteBounds() const;

    bool isColliding(int x, int y) const;
    bool toucheDecor(sf::FloatRect rectangle) const;
    bool positionSure(sf::FloatRect rectangle) const;
    int getNombreCollectibles() const;
    void drawAnneauxInterface(sf::RenderWindow& window);
    void recommencerMobiles();
    void draw(sf::RenderWindow& window);
};

#endif
