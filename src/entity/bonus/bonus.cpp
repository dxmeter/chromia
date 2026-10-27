#include "bonus.h"
#include <iostream>

Bonus::Bonus() : sprite(texture)
{
}

void Bonus::configurer(sf::Vector2f position, int newType)
{
    type = newType;
    isCollected = false;

    if (type == 1)
    {
        if (!texture.loadFromFile(RESOURCES_DIR "/collectible/bonus_vie_sprite_sheet.png"))
        {
            return;
        }
    }
    else if (type == 2)
    {
        if (!texture.loadFromFile(RESOURCES_DIR "/collectible/bonus_vitesse_sprite_sheet.png"))
        {
            return;
        }
    }

    sprite.setTexture(texture, true);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 512, 512 }));
    sprite.setScale({ 0.10f, 0.10f });
    sprite.setPosition(position);
    anim.restart();
}

void Bonus::update(float deltaTime)
{
    sf::IntRect rectangle({ 0, 0 }, { 512, 512 });
    int image = static_cast<int>(anim.getElapsedTime().asSeconds() / 0.15f) % 4;

    rectangle.position.x = image * 512;
    sprite.setTextureRect(rectangle);
}

sf::FloatRect Bonus::getBounds() const
{
    sf::FloatRect rectangle = sprite.getGlobalBounds();

    rectangle.position.x += rectangle.size.x * 0.30f;
    rectangle.position.y += rectangle.size.y * 0.30f;

    rectangle.size.x *= 0.40f;
    rectangle.size.y *= 0.40f;

    return rectangle;
}

int Bonus::getType()
{
    return type;
}

void Bonus::collecter()
{
    isCollected = true;
}

bool Bonus::estCollecte() const
{
    return isCollected;
}

void Bonus::recommencer()
{
    isCollected = false;
    anim.restart();
}

void Bonus::draw(sf::RenderWindow& window)
{
    if (isCollected)
        return;

    sf::Vector2f position = sprite.getPosition();

    sf::Sprite halo = sprite;
    halo.setColor(sf::Color(255, 245, 227, 25));

    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            halo.setPosition({
                position.x + x,
                position.y + y
                });

            window.draw(halo);
        }
    }

    window.draw(sprite);
}