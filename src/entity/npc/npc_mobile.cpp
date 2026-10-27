#include "npc_mobile.h"
#include <iostream>

NPCMobile::NPCMobile() : sprite(texture)
{
    limiteGauche = 550.f;
    limiteDroite = 800.f;

    if (!texture.loadFromFile(RESOURCES_DIR "/player/npc_mobile_sprite_sheet.png")) {
        return;
    }

    sprite.setTexture(texture, true);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 512, 512 }));
    sprite.setScale({ 0.30f, 0.30f });
    sprite.setPosition({ 1000.f, 1420 });

    sonPapillonCharge = bufferPapillon.loadFromFile(RESOURCES_DIR "/sound/moth.wav");
    sonPapillon.setVolume(75.f);
}

void NPCMobile::configurer(sf::Vector2f position, float gauche, float droite)
{
    sprite.setPosition(position);
    positionDepart = position;
    limiteGauche = gauche;
    limiteDroite = droite;
    vitesseX = 80.f;
}

void NPCMobile::update(float deltaTime)
{
    sprite.move({ vitesseX * deltaTime, 0.f });
    sf::Vector2f position = sprite.getPosition();

    if (position.x >= limiteDroite)
    {
        sprite.setPosition({ limiteDroite, position.y });
        vitesseX = -80.f;
    }
    if (position.x <= limiteGauche)
    {
        sprite.setPosition({ limiteGauche, position.y });
        vitesseX = 80.f;
    }

    sf::IntRect sub_npc({ 0, 0 }, { 512, 512 });
    sub_npc.position.x = (static_cast<int>(anim.getElapsedTime().asSeconds() / 0.15f) % 4) * 512;
    sprite.setTextureRect(sub_npc);

    // Audio
    if (sonPapillon.getStatus() != sf::Sound::Status::Playing)
    {
        sonPapillon.play();
    }
}

sf::FloatRect NPCMobile::getBounds() const
{
    sf::FloatRect rectangle = sprite.getGlobalBounds();

    rectangle.position.x += rectangle.size.x * 0.35f;
    rectangle.position.y += rectangle.size.y * 0.35f;

    rectangle.size.x *= 0.30f;
    rectangle.size.y *= 0.30f;

    return rectangle;
}

void NPCMobile::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect NPCMobile::getZonePatrouille() const
{
    sf::FloatRect zone = getBounds();
    float decalage = zone.position.x - sprite.getPosition().x;
    zone.position.x = limiteGauche + decalage;
    zone.size.x += limiteDroite - limiteGauche;
    return zone;
}

void NPCMobile::recommencer()
{
    sprite.setPosition(positionDepart);
    vitesseX = 80.f;
    anim.restart();
}
